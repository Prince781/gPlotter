AC_INIT([gplotter], [0.1a], [princetonf@optonline.net])

AM_INIT_AUTOMAKE([foreign -Wno-portability])
LT_INIT

# Check for required programs and packages
AC_PROG_CC
AM_PROG_VALAC

PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GOBJECT], [gobject-2.0])
PKG_CHECK_MODULES([VALA], [libvala-0.30 >= 0.29.3])

# GObject introspection
GOBJECT_INTROSPECTION_CHECK([1.30.0])

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 src/core/Makefile])

AC_OUTPUT

echo "
	${PACKAGE_NAME} ${PACKAGE_VERSION}
	Installation directory prefix: ${prefix}
	Compilation command: '${CC} ${CFLAGS}'
"
