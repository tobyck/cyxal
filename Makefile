gmp-arm = gmp/arm/libgmp.a
gmp-x86 = gmp/x86/libgmp.a
arch = $(shell uname -m)
gmp = $(gmp-x86)



ifeq ($(arch), "armv7l")
	gmp = $(gmp-arm)
endif
ifeq ($(arch), "arm64")
	gmp = $(gmp-arm)
endif
	
build: src
	clang src/*.c src/**/*.c $(gmp) -o build/cyxal

run: build/cyxal
	make build --always-make
	./build/cyxal

clean:
	rm build/*
