.PHONY: build
build:
	mkdir -p build
	cd build && cmake .. && cmake --build .

.PHONY: test
test:
	./build/http-server --test

.PHONY: run
run:
	./build/http-server

.PHONY: clean
clean:
	rm -rf build
