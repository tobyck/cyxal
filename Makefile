build: src
	gcc src/*.c -o build/cyxal

run: build/cyxal
	make build
	./build/cyxal