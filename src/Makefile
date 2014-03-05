# makefile
CC=gcc
PROGRAM=gplotter
BIN=bin
CFLAGS=-std=c99 -lm
# resources
CFILES=$(shell find . -type f -name '*.c')

all: $(CFILES)
	$(CC) $(CFLAGS) $(CFILES)
	@if [ ! -d $(BIN) ]; then mkdir $(BIN); fi
	@mv a.out $(BIN)/$(PROGRAM)

clean:
	rm -f $(BIN)/$(PROGRAM)
