build: src
	gcc src/*.c src/*/*.c gmp/libgmp.a -o build/cyxal

run: build/cyxal
	make build --always-make
	./build/cyxal

clean:
	rm build/*