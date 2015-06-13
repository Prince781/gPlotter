CFLAGS=-D_GNU_SOURCE -std=gnu11 $(shell pkg-config --cflags gtk+-3.0) -g
LDFLAGS=-lm -lreadline -ldyncall_s $(shell pkg-config --libs gtk+-3.0)
PROGRAM=gPlotter
SOURCES=$(shell find . -type f -name \*.c)
OBJECTS=$(SOURCES:%.c=%.o)

all: $(OBJECTS)
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(PROGRAM) $(LDFLAGS)
	@echo " CC $(PROGRAM)"

$(OBJECTS): %.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo " CC $@"

clean:
	$(RM) $(OBJECTS) $(PROGRAM)
	$(RM) resources.c
