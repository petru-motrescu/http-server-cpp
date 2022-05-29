# http-server-cpp

A toy C++ HTTP server for educational purposes. 


## Prerequisites
```
brew install cmake
```


## How to run
```
make build
make run
curl localhost:8080
```


## References
- [HTTP/1.1 standard](https://datatracker.ietf.org/doc/html/rfc7230)
- [Beej's Guide to Network Programming](http://beej.us/guide/bgnet/html/)
- [Wikipedia > BSD sockets](https://en.wikipedia.org/wiki/Berkeley_sockets)
- [FreeBSD > sockets](https://docs.freebsd.org/en/books/developers-handbook/sockets/) 
- [GNU > sockets](https://www.gnu.org/software/libc/manual/html_node/Sockets.html)
- [IBM > socket programming](https://www.ibm.com/docs/en/i/7.4?topic=communications-socket-programming)
- [IBM > socket pitfalls](https://developer.ibm.com/tutorials/l-sockpit/)

Mac OS man pages:
- [socket()](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/socket.2.html#//apple_ref/doc/man/2/socket)
- [setsockopt()](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/getsockopt.2.html#//apple_ref/doc/man/2/setsockopt)
- [bind()](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/bind.2.html#//apple_ref/doc/man/2/bind)
