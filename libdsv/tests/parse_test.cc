#include <boost/test/unit_test.hpp>

#include <boost/filesystem.hpp>

#include <dsv_parser.h>
#include "test_detail.h"

#include <errno.h>
#include <stdio.h>

#include <string>

/** \file
 *  \brief Unit tests for basic parsing
 */




namespace dsv {
namespace test {


namespace b = boost;
namespace fs = boost::filesystem;



namespace detail {





}


BOOST_AUTO_TEST_SUITE( parse_suite )

/** \test Create and destroy parser object
 */
BOOST_AUTO_TEST_CASE( parser_create )
{
  dsv_parser_t parser;
  dsv_parser_create(&parser);

  dsv_operations_t operations;
  dsv_operations_create(&operations);

  detail::record_context context;
  dsv_set_record_callback(detail::record_callback,&context,operations);

  fs::path filename(detail::testdatadir/"empty.csv");

  int result = dsv_parse(filename.string().c_str(),parser,operations);

  BOOST_REQUIRE_MESSAGE(result == 0,
    "dsv_parse failed on '" << filename << "' with exit code " << result);

  dsv_operations_destroy(operations);
  dsv_parser_destroy(parser);
}



BOOST_AUTO_TEST_SUITE_END()

}
}
