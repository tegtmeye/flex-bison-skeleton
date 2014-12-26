master_suite=$(top_srcdir)/tests/master_suite.cc
additional_libs= \
	$(BOOST_REGEX_LIBS) \
	$(BOOST_UNIT_TEST_FRAMEWORK_LIBS) \
	$(BOOST_SYSTEM_LIBS)

check_PROGRAMS= \
        create_test

AM_CPPFLAGS=-pedantic -Wno-long-long -ansi -Wall \
        -I$(top_srcdir) \
        -I$(top_srcdir)/libdsv/src \
        $(BOOST_CPPFLAGS)

AM_LDFLAGS= \
        $(top_builddir)/libdsv/src/libdsv.la \
        $(BOOST_LDFLAGS) \
        $(BOOST_REGEX_LDFLAGS) \
        $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)

create_test_SOURCES=$(master_suite) \
        create_test.cc
create_test_LDADD=$(additional_libs)

TESTS=\
        create_test

CLEANFILES=\
	create_test.log \
	create_test.trs \
	test-suite.log


