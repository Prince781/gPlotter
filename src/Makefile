CC=gcc
CFLAGS=-std=c11
LDFLAGS=-lm -lreadline
PROGRAM=gPlotter
SOURCES=$(shell find . -type f -name \*.c)
OBJECTS=$(SOURCES:%.c=%.o)

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(PROGRAM) $(LDFLAGS)

$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(PROGRAM)
