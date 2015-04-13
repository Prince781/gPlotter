GCR=glib-compile-resources
CFILE=resources.c

all:
	$(GCR) gplotter.gresource.xml --target=$(CFILE) --generate-source

clean:
	$(RM) $(CFILE)
