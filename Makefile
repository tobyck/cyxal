COMPILER = clang
OUT_DIR = ./build/mac

ifeq ($(shell uname -s), Linux)
	OUT_DIR = ./build/linux
endif

OUTPUT_FILE = $(OUT_DIR)/cyxal

build: src
	$(COMPILER) src/*.c src/**/*.c -lgmp -o $(OUTPUT_FILE)

run: $(OUTPUT_FILE)
	@make build
	$(OUTPUT_FILE)

clean:
	rm build/**/*

install-gmp:
ifndef DIR
	@echo "Please specify a directory with DIR=<path/for/gmp/>"
	@exit 1;
endif
	curl https://gmplib.org/download/gmp/gmp-6.2.1.tar.xz --output $(DIR)/gmp-6.2.1.tar.xz
	tar -xf $(DIR)/gmp-6.2.1.tar.xz
	cd gmp-6.2.1
	./configure
	make
	make install

include-gmp:
	cp $(DIR)/.libs/libgmp.10.dylib /usr/local/lib
	cp $(DIR)/gmp.h /usr/local/include