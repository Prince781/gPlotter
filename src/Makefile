# makefile
CC=gcc
PROGRAM=gplotter
BIN=bin
CFLAGS=-std=c99 -lm -o $(BIN)/$(PROGRAM)
# resources
CFILES=$(shell find . -type f -name '*.c')

all: $(CFILES)
	$(CC) $(CFLAGS) $(CFILES) 

clean:
	rm -f $(BIN)/$(PROGRAM)
