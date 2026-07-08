CC = gcc
CFLAGS = -Wall -Wextra

bin/gofish: build/main.o build/deck.o build/game.o build/stack.o build/bot.o build/tui.o
	$(CC) build/main.o build/deck.o build/game.o build/stack.o build/bot.o build/tui.o -o bin/gofish

build/main.o: src/main.c src/game.h src/deck.h src/stack.h src/tui.h
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o

build/deck.o: src/deck.c src/deck.h
	$(CC) $(CFLAGS) -c src/deck.c -o build/deck.o

build/game.o: src/game.c src/game.h src/deck.h src/stack.h src/tui.h
	$(CC) $(CFLAGS) -c src/game.c -o build/game.o

build/stack.o: src/stack.c src/stack.h src/deck.h
	$(CC) $(CFLAGS) -c src/stack.c -o build/stack.o

build/tui.o: src/tui.c
	$(CC) $(CFLAGS) -c src/tui.c -o build/tui.o

build/bot.o: src/bot.c
	$(CC) $(CFLAGS) -c src/bot.c -o build/bot.o

clean:
	rm -f build/*.o bin/gofish
