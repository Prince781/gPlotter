## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir). \
	$(GDK_CFLAGS)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(GPLOTTER_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gplotter

gplotter_SOURCES = \
	main.c \
	gplotter.css \
	gplotter.h \
	gp_math.c \
	drawing.c

gplotter_CFLAGS = -std=gnu11 

gplotter_LDFLAGS = \
	-Wl,--export-dynamic

gplotter_LDADD = $(GPLOTTER_LIBS) \
	$(GDK_LIBS)


