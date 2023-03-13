GMP_ARM = gmp/arm/libgmp.a # path to lib file for arm
GMP_X86 = gmp/x86/libgmp.a # path for x86

ifeq ($(ARCH), arm) # if running arm
	GMP_PATH = $(GMP_ARM) # change path to lib file
	OUTPUT_FILE = ./build/arm/cyxal
else ifeq ($(ARCH), x86)
	GMP_PATH = $(GMP_X86)
    OUTPUT_FILE = build/x86/cyxal
endif

build: src
ifndef ARCH
	@echo "Please specify an architecture to build for."
	@exit 1
endif
	@echo "Building for $(ARCH)..."
	@clang src/*.c src/**/*.c $(GMP_PATH) -o $(OUTPUT_FILE)
	@echo "Done."

run: $(OUTPUT_FILE)
	@make build
	$(OUTPUT_FILE)

clean:
	rm build/**/*