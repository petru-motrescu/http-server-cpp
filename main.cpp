#include <iostream>
#include <vector>
#include <sstream>
#include <string_view>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

const int BACKLOG = 4;
const int SERVER_PORT = 8080;
const int REQUEST_MAX_SIZE = 1024;

std::string build_response() {
    std::stringstream response_lines {};
    response_lines << "HTTP/1.1 200 OK" << std::endl;
    response_lines << "Content-Length: 13" << std::endl;
    response_lines << "Content-Type: text/plain; charset=utf-8" << std::endl;
    response_lines << std::endl;
    response_lines << "Hello World!" << std::endl;
    return response_lines.str();
}

int open_socket() {
    return socket(
        PF_INET,        // domain
        SOCK_STREAM,    // type
        0);             // protocol
}

int enable_address_reuse(int socket_id) {
    int enabled = 1;
    return setsockopt(
        socket_id,      // socket
        SOL_SOCKET,     // level
        SO_REUSEADDR,   // option_name
        &enabled,       // option_value
        sizeof(int));   // option_len
}

int bind_address(int socket_id, int port) {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY); // use own IPv4 address
    address.sin_port = htons(port);

    return bind(
        socket_id,                              // socket
        (struct sockaddr *) &address,           // address
        sizeof(address));                       // address_len
}

int accept_client_connection(int socket_id) {
    struct sockaddr_storage client_address;
    socklen_t client_address_size = sizeof(client_address);
    return accept(
        socket_id,                              // socket
        (struct sockaddr *) &client_address,    // address
        &client_address_size);                  // adress_len
}

int write_response(int socket_id, const std::string& response) {
    return write(socket_id, response.c_str(), response.length());
}

enum class ServerResult {
    OK,
    FAILED_TO_OPEN_MAIN_SOCKET,
    FAILED_TO_SET_ADDRESS_REUSE,
    FAILED_TO_BIND_ADDRESS,
    FAILED_TO_LISTEN_FOR_CONNECTIONS,
    FAILED_TO_ACCEPT_CLIENT_CONNECTION,
    FAILED_TO_READ_REQUEST,
    FAILED_TO_WRITE_RESPONSE
};

std::string to_string(ServerResult result) {
    switch (result)
    {
        case ServerResult::OK:                                  return "OK";
        case ServerResult::FAILED_TO_OPEN_MAIN_SOCKET:          return "Failed to open main socket.";
        case ServerResult::FAILED_TO_SET_ADDRESS_REUSE:         return "Failed to enable address reuse.";
        case ServerResult::FAILED_TO_BIND_ADDRESS:              return "Failed to bind address.";
        case ServerResult::FAILED_TO_LISTEN_FOR_CONNECTIONS:    return "Failed to listen for connections.";
        case ServerResult::FAILED_TO_ACCEPT_CLIENT_CONNECTION:  return "Failed to accept client connection.";
        case ServerResult::FAILED_TO_READ_REQUEST:              return "Failed to read client request.";
        case ServerResult::FAILED_TO_WRITE_RESPONSE:            return "Failed to write response.";
    }
}

ServerResult run_server(int port) {
    int main_socket = open_socket();
    if (main_socket < 0) {
        return ServerResult::FAILED_TO_OPEN_MAIN_SOCKET;
    }

    if (enable_address_reuse(main_socket) < 0) {
        return ServerResult::FAILED_TO_SET_ADDRESS_REUSE;
    }

    if (bind_address(main_socket, port) < 0) {
        return ServerResult::FAILED_TO_BIND_ADDRESS;
    }

    if (listen(main_socket, BACKLOG) < 0) {
        return ServerResult::FAILED_TO_LISTEN_FOR_CONNECTIONS;
    }

    while (true) {
        std::cout << std::endl << "📡 Waiting for requests on port " << port << std::endl;

        int client_socket = accept_client_connection(main_socket);
        if (client_socket < 0) {
            return ServerResult::FAILED_TO_ACCEPT_CLIENT_CONNECTION;
        }

        char buffer[REQUEST_MAX_SIZE];
        memset(buffer, 0, REQUEST_MAX_SIZE);
        int read_result = read(client_socket, buffer, REQUEST_MAX_SIZE - 1);
        if (read_result < 0) {
            return ServerResult::FAILED_TO_READ_REQUEST;
        }

        if (read_result > 0) {
            std::cout << std::endl << "📟 Received request: " << std::endl << buffer << std::endl;

            if (write_response(client_socket, build_response()) < 0) {
                return ServerResult::FAILED_TO_WRITE_RESPONSE;
            }
        }

        close(client_socket);
    }

    close(main_socket);
    return ServerResult::OK;
}

int main() {
    std::cout << "👋 Hi" << std::endl;

    auto result = run_server(SERVER_PORT);
    if (result == ServerResult::OK) {
        std::cout << "✅ All good" << std::endl;
        return 0;
    } else {
        std::cout << "😭 Got an error: " << to_string(result) << std::endl;
        return -1;
    }
}
