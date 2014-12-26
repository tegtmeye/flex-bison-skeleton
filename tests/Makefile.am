noinst_HEADERS=

additional_cppflags= \
	-pedantic -Wno-long-long -ansi -Wall \
	-I$(top_srcdir) \
	-DTESTDATA_DIR='$(srcdir)/data' \
	$(BOOST_CPPFLAGS)

additional_libs= \
        $(BOOST_REGEX_LIBS) \
        $(BOOST_UNIT_TEST_FRAMEWORK_LIBS) \
        $(BOOST_SYSTEM_LIBS) \
	$(HDF5_LIBS)

additional_ldflags= \
        $(BOOST_LDFLAGS) \
        $(BOOST_REGEX_LDFLAGS) \
        $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS) \
        $(BOOST_SYSTEM_LDFLAGS)

libdsv_testdir=$(top_srcdir)/libdsv/tests
libdsv_tests=\
	$(libdsv_testdir)/test_detail.h \
	$(libdsv_testdir)/create_test.cc \
	$(libdsv_testdir)/parse_test.cc

check_PROGRAMS=libdsv_test

libdsv_test_SOURCES=\
	master_suite.cc \
	$(libdsv_tests)

libdsv_test_CPPFLAGS=$(additional_cppflags)

libdsv_test_LDADD=\
	$(top_builddir)/libdsv/src/libdsv.la \
	$(additional_libs)

libdsv_test_LDFLAGS=$(additional_ldflags)

TESTS=\
	test_wrapper.sh

EXTRA_DIST=\
	test_wrapper.sh

CLEANFILES=\
	libdsv_test.xml

DISTCLEANFILES=\
	libdsv_test.xml
