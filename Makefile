build: src
	gcc src/main.c src/*/*.c gmp/libgmp.a -o build/cyxal

run: build/cyxal
	make build -B
	./build/cyxal

clean:
	rm build/*
