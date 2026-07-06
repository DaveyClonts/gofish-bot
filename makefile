CC = gcc
CFLAGS = -Wall -Wextra

executable: main.o deck.o bot.o
	$(CC) -o executable main.o deck.o