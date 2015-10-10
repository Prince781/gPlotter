#!/bin/sh
# generates initial makefiles

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

(test -f $srcdir/src/main.c) || {
	echo -n "**Error**: directory "\`$srcdir\'" does not look like the"
	echo " top-level gplotter directory"
	exit 1
}

datadir=$srcdir/data
test -d "$datadir" || mkdir -p $datadir

libdir=$srcdir/libs
test -d "$libdir" || mkdir -p $libdir

touch ChangeLog
touch INSTALL

aclocal --install -I build/autotools || exit 1
autoreconf --force --install -Wno-portability || exit 1

# for use in gnome-builder
if [ "$NOCONFIGURE" = "" ]; then
        $srcdir/configure "$@" || exit 1

        if [ "$1" = "--help" ]; then exit 0 else
                echo "Now type \`make\' to compile" || exit 1
        fi
else
        echo "Skipping configure process."
fi

