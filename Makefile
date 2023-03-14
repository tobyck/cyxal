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