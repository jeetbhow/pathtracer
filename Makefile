.DEFAULT_GOAL := build

.PHONY: build run test

build:
	@test -f build/meson-private/coredata.dat || meson setup build
	meson compile -C build

run: build
	./build/pathtracer

test: build
	meson test -C build --print-errorlogs
