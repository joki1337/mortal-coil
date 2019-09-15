.PHONY: build

build:
	mkdir -p build
	cd build && gcc ../src/main.c

run:
	build/a.out < levels/level0050
