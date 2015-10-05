SUBDIRS = core

bin_PROGRAMS = gplotter

gplotter_SOURCES = main.c
gplotter_CFLAGS = -g $(GOBJECT_CFLAGS)
gplotter_LDADD = core/libgplotter-core.la $(GOBJECT_LIBS) -lm
