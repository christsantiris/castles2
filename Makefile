.PHONY: build run test

build:
	cmake -S . -B build && cmake --build build

run: build
	./build/castle3

test: build
	./build/castles3_tests