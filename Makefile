.PHONY: all get-deps test

all:
	@mkdir build && cd build && cmake .. && make thread

get-deps:
	@git submodule update --remote --init

test:
	@./build/bin/thread/thread_unittests
