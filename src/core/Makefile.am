lib_LTLIBRARIES = libgplotter-core.la

libgplotter_core_la_SOURCES = \
      gp-context.c \
      gp-function.c \
      gp-native-function.c \
      gp-variable.c \
      native.c \
      core.c \
      stack.c

libgplotter_core_la_CFLAGS = -g $(GOBJECT_CFLAGS)
libgplotter_core_la_LIBADD = $(GOBJECT_LIBS) -lm

CLEANFILES = libgplotter-core.{la,so}

