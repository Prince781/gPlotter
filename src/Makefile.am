## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(GPLOTTER_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gplotter

gplotter_SOURCES = \
	main.c

gplotter_LDFLAGS = \
	-Wl,--export-dynamic

gplotter_LDADD = $(GPLOTTER_LIBS)


