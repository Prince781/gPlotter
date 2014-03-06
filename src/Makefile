# makefile
CC=gcc
PROGRAM=gplotter
BIN=bin
CFLAGS := $(shell pkg-config --cflags glib-2.0 gtk+-3.0) -std=c99 -lm
LDFLAGS := $(shell pkg-config --libs glib-2.0 gtk+-3.0)
# resources
CFILES=$(shell find . -type f -name '*.c')

all: $(CFILES)
	$(CC) $(CFLAGS) $(CFILES) $(LDFLAGS)
	@if [ ! -d $(BIN) ]; then mkdir $(BIN); fi
	@mv a.out $(BIN)/$(PROGRAM)

clean:
	rm -f $(BIN)/$(PROGRAM)
