CC = gcc
CFLAGS = -Iinclude
LDFLAGS = -Llib -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

SRC = $(wildcard src/**/*.c) src/*.c
OUT = build/rts

all:
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(OUT)

clean:
	rm -f $(OUT)
