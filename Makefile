CC = clang
OUT_DIR = ./build/debug/

OUTPUT_FILE = $(OUT_DIR)/cyxal

build: src
	$(CC) -Wall -lgmp src/*.c src/**/*.c -o $(OUTPUT_FILE) $(FLAGS)

run: $(OUTPUT_FILE)
	make build
	$(OUTPUT_FILE)

debug:
	make build FLAGS=-g
	lldb $(OUTPUT_FILE)

clean:
	rm build/**/*

install-gmp:
ifndef DIR
	@echo "Please specify a directory to install GMP in with 'path/for/gmp'"
	@exit 1
endif
	curl https://gmplib.org/download/gmp/gmp-6.2.1.tar.xz --output $(DIR)/gmp-6.2.1.tar.xz
	tar -xf $(DIR)/gmp-6.2.1.tar.xz -C $(DIR)
	cd $(DIR)/gmp-6.2.1 && ./configure && make && make check && sudo make install && rm $(DIR)/gmp-6.2.1.tar.xz

include-gmp:
	cp $(DIR)/.libs/libgmp.10.dylib /usr/local/lib
	cp $(DIR)/gmp.h /usr/local/include