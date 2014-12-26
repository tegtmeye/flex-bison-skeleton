master_suite=$(top_srcdir)/tests/master_suite.cc

additional_test_cppflags= \
	-pedantic -Wno-long-long -ansi -Wall \
        -I$(top_srcdir) \
        -I$(top_srcdir)/libdsv/src \
	-DTESTDATA_DIR='$(srcdir)/data' \
        $(BOOST_CPPFLAGS)

additional_test_libs= \
	$(BOOST_REGEX_LIBS) \
	$(BOOST_UNIT_TEST_FRAMEWORK_LIBS) \
	$(BOOST_SYSTEM_LIBS) \
	$(BOOST_FILESYSTEM_LIBS)

additional_test_ldflags=\
        $(top_builddir)/libdsv/src/libdsv.la \
        $(BOOST_LDFLAGS) \
        $(BOOST_REGEX_LDFLAGS) \
        $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS) \
	$(BOOST_SYSTEM_LDFLAGS) \
	$(BOOST_FILESYSTEM_LDFLAGS)

check_PROGRAMS= \
        create_test \
	parse_test

create_test_SOURCES=$(master_suite) \
	test_detail.h \
        create_test.cc
create_test_CPPFLAGS=$(additional_test_cppflags)
create_test_LDADD=$(additional_test_libs)
create_test_LDFLAGS=$(additional_test_ldflags)

parse_test_SOURCES=$(master_suite) \
	test_detail.h \
        parse_test.cc
parse_test_CPPFLAGS=$(additional_test_cppflags)
parse_test_LDADD=$(additional_test_libs)
parse_test_LDFLAGS=$(additional_test_ldflags)

TESTS=\
        create_test \
	parse_test

CLEANFILES=\
	create_test.log \
	create_test.trs \
	parse_test.log \
	parse_test.trs \
	test-suite.log

EXTRA_DIST=\
	data/empty.csv

