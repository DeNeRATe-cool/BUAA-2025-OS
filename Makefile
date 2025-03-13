all: check
	gcc -Isrc/include src/main.c src/output.c -o out/main

check: check.c
	gcc -c check.c -o check.o

run: all
	./out/main

clean:
	rm -f check.o ./out/main
