# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(libdsv, .1.0)
AC_CONFIG_SRCDIR(Makefile.am)
AM_CONFIG_HEADER(config.h)
AC_CONFIG_AUX_DIR(config)
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

AC_LIBTOOL_DLOPEN

AC_PROG_LIBTOOL
dnl Configure libltdl                                                           
AM_PROG_LEX
if test "x$LEX" != xflex; then
   LEX="$SHELL $missing_dir/missing flex"
   AC_SUBST([LEX_OUTPUT_ROOT], [lex.yy])
   AC_SUBST([LEXLIB], [''])
fi

AC_PROG_YACC
AC_PROG_LN_S

# We need boost
BOOST_REQUIRE([1.43])
BOOST_TEST
BOOST_SYSTEM
BOOST_FILESYSTEM

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_HEADER([unistd.h],[],
        [AC_DEFINE([YYNO_UNISTD_H],[1],[Don't include unistd.h])])
AC_CHECK_FUNC([read],[],
        [AC_DEFINE([NO_POSIX_READ],[1],[Don't use read function])])


# Check for single testsuite
AC_ARG_WITH([single-testsuite],
[AS_HELP_STRING([--with-single-testsuite],
        [single testsuite with XML output @<:@default=no@:>@])],
[case "${withval}" in
  yes) ;;
  no) ;;
  *) AC_MSG_ERROR([bad value ${withval} for --with-single-testsuite]) ;; 
esac],[with_single_testsuite=no]) 
AM_CONDITIONAL([SINGLE_TESTSUITE], [test "x$with_single_testsuite" == xyes]) 



AC_CONFIG_FILES( \
 libdsv.pc \
 Makefile \
 libdsv/Makefile \
 libdsv/src/Makefile \
 libdsv/tests/Makefile \
 tests/Makefile \
 docs/Makefile)

AC_OUTPUT
