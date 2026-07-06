CC = gcc
CFLAGS = -Wall -Wextra

bin/gofish: build/main.o build/deck.o
	$(CC) build/main.o build/deck.o -o bin/gofish

build/main.o: src/main.c src/deck.h
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o

build/deck.o: src/deck.c src/deck.h
	$(CC) $(CFLAGS) -c src/deck.c -o build/deck.o

clean: 
	rm -f build/*.o bin/gofish
