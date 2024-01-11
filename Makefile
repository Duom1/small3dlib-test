SOURCES = main.c
CFLAGS = -O3 -march=native -Wall -Werror -pedantic -std=c99
LDFLAGS = -lSDL2

def:
	$(CC) -o o $(SOURCES) $(CFLAGS) $(LDFLAGS)
