COMPILER = clang
OUT_DIR = ./build/mac

ifeq ($(shell uname -s), Linux)
	OUT_DIR = ./build/linux
endif

OUTPUT_FILE = $(OUT_DIR)/cyxal

install-gmp:
ifndef DIR
	@echo "Please specify a directory to install GMP in with 'path/for/gmp'"
	@exit 1;
endif
	curl https://gmplib.org/download/gmp/gmp-6.2.1.tar.xz --output $(DIR)/gmp-6.2.1.tar.xz
	tar -xf $(DIR)/gmp-6.2.1.tar.xz -C $(DIR)
	cd $(DIR)/gmp-6.2.1 && ./configure && make && make check && sudo make install && rm $(DIR)/gmp-6.2.1.tar.xz

build: src
	$(COMPILER) src/*.c src/**/*.c -lgmp -o $(OUTPUT_FILE)

run: $(OUTPUT_FILE)
	@make build
	$(OUTPUT_FILE)

clean:
	rm build/**/*