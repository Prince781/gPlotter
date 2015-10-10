DISTCHECK_CONFIGURE_FLAGS = --enable-introspection
SUBDIRS = core

lib_LTLIBRARIES = libgplotter.la

libgplotter_la_SOURCES = main.c
libgplotter_la_CFLAGS = -g $(GOBJECT_CFLAGS)
libgplotter_la_LIBADD = \
	core/libgplotter-core.la \
	$(GOBJECT_LIBS) -lm

CLEANFILES = libgplotter.{la,so}

all_sources = \
	core/*.c

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --symbol-prefix=gp --warn-all
INTROSPECTION_COMPILER_ARGS =

if HAVE_INTROSPECTION
introspection_sources = $(all_sources)
introspection_sources += $(all_sources:.c=.h)

GP-0.1.gir: libgplotter.la
GP_0_1_gir_NAMESPACE = GP
GP_0_1_gir_INCLUDES = GObject-2.0 GLib-2.0
GP_0_1_gir_CFLAGS = $(INCLUDES)
GP_0_1_gir_LIBS = libgplotter.la
GP_0_1_gir_FILES = $(introspection_sources)
INTROSPECTION_GIRS += GP-0.1.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif
