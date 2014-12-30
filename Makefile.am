ACLOCAL_AMFLAGS = -I m4

if SINGLE_TESTSUITE
MAYBE_TEST=tests
else
MAYBE_TEST=
endif

include_HEADERS= \
	dsv_parser.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libdsv.pc

SUBDIRS= \
	docs \
	libdsv \
	$(MAYBE_TEST)

EXTRA_DIST=\
	COPYING \
	ChangeLog \
	INSTALL \
	README
