# makefile
CC=gcc
CFLAGS=-std=c99 -lm
PROGRAM=gplotter
BIN=bin
# resources
CFILES=$(shell find . -type f -name '*.c')
HFILES=$(shell find . -type f -name '*.h')

all: $(HFILES) $(CFILES)
	$(CC) $(CFLAGS) $(HFILES) $(CFILES)
	@mv a.out $(BIN)/$(PROGRAM)
	@mv $(HFILES).gch $(BIN)/$(HFILES).gch

clean:
	rm -f $(BIN)/$(PROGRAM)
	rm -f $(BIN)/$(HFILES).gch
