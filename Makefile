CC = clang
DB = lldb
OUT_DIR = build
OUT_FILE = $(OUT_DIR)/cyxal
LEAKS_FLAGS = --atExit
FILES_FOR_TESTS = $(filter-out src/main.c, $(wildcard tests/*.c src/*.c src/**/*.c))

# adjust flags for `leaks` if trying to find source of memory leaks
ifeq ($(MallocStackLogging),1)
ifneq ($(wildcard $(OUT_FILE).dSYM),"")
	LEAKS_FLAGS = --atExit --list --quiet
endif
endif

# build targets

build: src
	$(CC) -Wall -lgmp -lm src/*.c src/**/*.c -o $(OUT_FILE) $(FLAGS)

debug-build: src
	make build FLAGS="-g"

run: $(OUT_FILE)
	./$(OUT_FILE)

# debug targets

debug: $(OUT_FILE)
	$(DB) $(OUT_FILE)

leak-check-mac: $(OUT_FILE)
	leaks $(LEAKS_FLAGS) $(FLAGS) -- ./$(OUT_FILE)

clean:
	find build/*/* ! -name '*.md' -delete

# gmp setup

install-gmp:
ifndef DIR
	@echo "Please specify a directory to install GMP in with 'path/for/gmp'"
	@exit 1
endif
	curl https://gmplib.org/download/gmp/gmp-6.2.1.tar.xz --output $(DIR)/gmp-6.2.1.tar.xz
	tar -xf $(DIR)/gmp-6.2.1.tar.xz -C $(DIR)
	cd $(DIR)/gmp-6.2.1
	./configure
	make
	make check
	sudo make install
	rm $(DIR)/gmp-6.2.1.tar.xz

include-gmp:
	cp $(DIR)/.libs/libgmp.10.dylib /usr/local/lib
	cp $(DIR)/gmp.h /usr/local/include

# testing

build-tests: tests
	$(CC) -Wall -lgmp -lm $(FILES_FOR_TESTS) -o $(OUT_DIR)/test $(FLAGS)