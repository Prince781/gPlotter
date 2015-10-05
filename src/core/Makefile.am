DISTCHECK_CONFIGURE_FLAGS = --enable-introspection
lib_LTLIBRARIES = libgplotter-core.la

libgplotter_core_la_SOURCES = \
      gp-context.c \
      gp-function.c \
      gp-native-function.c \
      gp-variable.c \
      gp-repl.c \
      native.c \
      core.c \
      stack.c

libgplotter_core_la_CFLAGS = -g $(GOBJECT_CFLAGS)
libgplotter_core_la_LIBADD = $(GOBJECT_LIBS) -lm

CLEANFILES = libgplotter-core.{la,so}

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS = 
INTROSPECTION_SCANNER_ARGS = --symbol-prefix=gp --warn-all
INTROSPECTION_COMPILER_ARGS = 

if HAVE_INTROSPECTION
introspection_sources = $(libgplotter_core_la_SOURCES)
introspection_sources += $(libgplotter_core_la_SOURCES:.c=.h)

GP-0.1.gir: libgplotter-core.la
GP_0_1_gir_NAMESPACE = GP
GP_0_1_gir_INCLUDES = GObject-2.0 GLib-2.0
GP_0_1_gir_CFLAGS = $(INCLUDES)
GP_0_1_gir_LIBS = libgplotter-core.la
GP_0_1_gir_FILES = $(introspection_sources)
INTROSPECTION_GIRS += GP-0.1.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif
