#include <boost/test/unit_test.hpp>

#include <boost/filesystem.hpp>
#include <boost/scoped_array.hpp>

#include <dsv_parser.h>
#include "test_detail.h"

#include <errno.h>
#include <stdio.h>

#include <string>
#include <algorithm>

/** \file
 *  \brief Unit tests for basic parsing
 */




namespace dsv {
namespace test {


namespace b = boost;
namespace fs = boost::filesystem;



namespace detail {

std::string msg_log(dsv_parser_t parser, dsv_log_level log_level)
{
  size_t msg_len = dsv_parse_error(parser,dsv_log_all,0,0);
  boost::scoped_array<char> buf(new char[msg_len]);
  msg_len = dsv_parse_error(parser,dsv_log_all,buf.get(),msg_len);
  return std::string(buf.get());
}

inline void parser_destroy(dsv_parser_t *p)
{
  dsv_parser_destroy(*p);
}

inline void operations_destroy(dsv_operations_t *p)
{
  dsv_operations_destroy(*p);
}

boost::shared_ptr<dsv_parser_t> make_parser(void)
{
  std::auto_ptr<dsv_parser_t> temp(new dsv_parser_t());

  dsv_parser_create(temp.get());

  return boost::shared_ptr<dsv_parser_t>(temp.release(),&parser_destroy);
}

boost::shared_ptr<dsv_operations_t> make_operations(void)
{
  std::auto_ptr<dsv_operations_t> temp(new dsv_operations_t());

  dsv_operations_create(temp.get());

  return boost::shared_ptr<dsv_operations_t>(temp.release(),&operations_destroy);
}


}


BOOST_AUTO_TEST_SUITE( parse_suite )

/** \test Check to see if error fn recognizes uninitialized parsers
 */
BOOST_AUTO_TEST_CASE( noninitialzed_parser_error_fn_check )
{
  dsv_parser_t parser = {};

  size_t msg_len = dsv_parse_error(parser,dsv_log_all,0,0);

  BOOST_REQUIRE_MESSAGE(msg_len == 0,
    "dsv_parse_error recognizes an uninitialized parser object");
}

/** \test Check to see if error fn returns empty string on no-parse condition
 */
BOOST_AUTO_TEST_CASE( no_parse_parser_error_fn_check )
{
  dsv_parser_t parser = {};
  dsv_parser_create(&parser);

  size_t msg_len = dsv_parse_error(parser,dsv_log_all,0,0);

  BOOST_REQUIRE_MESSAGE(msg_len == 1,
    "dsv_parse_error returns a value of 1 on a parser without actually parsing");

  char buf[256];
  std::fill(buf,buf+256,'X');
  size_t ret_msg_len = dsv_parse_error(parser,dsv_log_all,buf,0);

  BOOST_REQUIRE_MESSAGE(ret_msg_len == 1,
    "dsv_parse_error returns a value of 1 when providing buffer of 0 len on a "
    "parser without actually parsing");

  BOOST_REQUIRE_MESSAGE(buf[0] == 'X',
    "dsv_parse_error did not modify the given buffer when provided a len parameter of 0 "
    "on a parser without actually parsing");

  ret_msg_len = dsv_parse_error(parser,dsv_log_all,buf,1);

  BOOST_REQUIRE_MESSAGE(ret_msg_len == 1,
    "dsv_parse_error returns a value of 1 when providing buffer of length 1 on a "
    "parser without actually parsing");

  BOOST_REQUIRE_MESSAGE(buf[0] == 0,
    "dsv_parse_error sets the empty string on the buffer when provided a length "
    "parameter of 1 on a parser without actually parsing");

  BOOST_REQUIRE_MESSAGE(buf[1] == 'X',
    "dsv_parse_error does not modify more of the provided buffer than it should when "
    "when provided a correct sized len parameter on a parser without actually parsing");

  ret_msg_len = dsv_parse_error(parser,dsv_log_all,buf,256);

  BOOST_REQUIRE_MESSAGE(ret_msg_len == 1,
    "dsv_parse_error returns a length parameter of 1 when providing buffer of oversized "
    "length on a parser without actually parsing");

  BOOST_REQUIRE_MESSAGE(buf[0] == 0,
    "dsv_parse_error returns the empty string on the provided buffer when provided "
    "an oversized len parameter");

  BOOST_REQUIRE_MESSAGE(buf[1] == 'X',
    "dsv_parse_error does not modify more of the provided buffer than it should when "
    "when provided an oversized len parameter on a parser without actually parsing");
}

/** \test Creation of operations object
 */
BOOST_AUTO_TEST_CASE( operations_create )
{
  dsv_operations_t operations = {};

  int err = dsv_operations_create(&operations);

  BOOST_REQUIRE_MESSAGE(err == 0,"dsv_operations_create succeeds");

  dsv_operations_destroy(operations);
}


/** \test Getting and setting of operations members
 */
BOOST_AUTO_TEST_CASE( record_getting_and_setting )
{
  dsv_operations_t operations = {};

  int err = dsv_operations_create(&operations);
  BOOST_REQUIRE_MESSAGE(err == 0,"dsv_operations_create succeeds");

  record_callback_t fn = dsv_get_record_callback(operations);
  BOOST_REQUIRE_MESSAGE(fn == 0,
    "dsv_get_record_callback returns zero for an unset record callback");

  void *context = dsv_get_record_context(operations);
  BOOST_REQUIRE_MESSAGE(context == 0,
    "dsv_get_record_context returns zero for an unset record context");

  detail::record_context record_context; // may throw

  err = dsv_set_record_callback(detail::record_callback,&record_context,operations);
  BOOST_REQUIRE_MESSAGE(err == 0,
    "dsv_set_record_callback succeeds");

  fn = dsv_get_record_callback(operations);
  BOOST_REQUIRE_MESSAGE(fn == detail::record_callback,
    "dsv_get_record_callback returns the just-set record callback");

  context = dsv_get_record_context(operations);
  BOOST_REQUIRE_MESSAGE(context == &record_context,
    "dsv_get_record_context returns the just-set record context");

  dsv_operations_destroy(operations);
}

/** \test Create and destroy parser object
 */
BOOST_AUTO_TEST_CASE( parser_create )
{
  dsv_parser_t parser = {};

  int err = dsv_parser_create(&parser);

  BOOST_REQUIRE_MESSAGE(err == 0,"dsv_parser_create succeeds");

  dsv_parser_destroy(parser);
}

/** \test Create and destroy parser object
 */
BOOST_AUTO_TEST_CASE( parse_empty_file )
{
  dsv_parser_t parser;
  assert(dsv_parser_create(&parser) == 0);
  boost::shared_ptr<dsv_parser_t> parser_sentry(&parser,detail::parser_destroy);

  dsv_operations_t operations;
  assert(dsv_operations_create(&operations) == 0);
  boost::shared_ptr<dsv_operations_t> operations_sentry(&operations,detail::operations_destroy);

  detail::record_context context;
  dsv_set_record_callback(detail::record_callback,&context,operations);

  fs::path filename(detail::testdatadir/"empty.csv");

  int result = dsv_parse(filename.string().c_str(),parser,operations);

  BOOST_REQUIRE_MESSAGE(result == 0,
    "dsv_parse failed on '" << filename << "' with message \""
      << detail::msg_log(parser,dsv_log_all) << "\"");

}



BOOST_AUTO_TEST_SUITE_END()

}
}
