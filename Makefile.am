## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

gplotterdocdir = ${prefix}/doc/gplotter
gplotterdoc_DATA = \
	README.md


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(gplotterdoc_DATA) \
	$(INTLTOOL_FILES)

AM_CFLAGS = -std=c11 

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(gplotterdocdir)
