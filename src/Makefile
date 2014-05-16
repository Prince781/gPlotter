# makefile
CC=gcc
PROGRAM=gplotter
BIN=bin
CFLAGS := $(shell pkg-config --cflags glib-2.0 gtk+-3.0) -g -Wall -std=c99 -lm
LDFLAGS := $(shell pkg-config --libs glib-2.0 gtk+-3.0)

all: window.o gpmath.o debug.o main.o
	@if [ ! -d $(BIN) ]; then mkdir $(BIN); fi
	@$(CC) $(CFLAGS) -o $(BIN)/$(PROGRAM) main.o window.o gpmath.o $(LDFLAGS)

main.o: main.c debug.h
	@$(CC) $(CFLAGS) -c main.c

debug.o: debug.c debug.h
	@$(CC) $(CFLAGS) -c debug.c

window.o: window.c window.h gplotter.h
	@$(CC) $(CFLAGS) -c window.c

gpmath.o: gpmath.c gpmath.h
	@$(CC) $(CFLAGS) -c gpmath.c

clean:
	rm -f *.o *~ $(BIN)/$(PROGRAM) 
