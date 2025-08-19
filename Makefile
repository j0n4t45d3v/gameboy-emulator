.PHONY: all build test test-resume

all: build

build: 
	bash ./build.sh

build-debug: 
	bash ./build.sh --debug

test-debug: build-debug
	gdb -tui ./build/tests

test: build
	ctest --test-dir build --output-on-failure

test-verbose: build
	./build/tests

