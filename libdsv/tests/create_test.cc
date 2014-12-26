#include <boost/test/unit_test.hpp>

#include <boost/filesystem.hpp>

#include <dsv_parser.h>
#include "test_detail.h"


#include <errno.h>
#include <stdio.h>

#include <string>

/** \file
 *  \brief Unit tests for creation
 */

namespace dsv {
namespace test {


namespace b = boost;
namespace fs = boost::filesystem;

namespace detail {




}


BOOST_AUTO_TEST_SUITE( create_suite )

/** \test Create and destroy parser object
 */
BOOST_AUTO_TEST_CASE( parser_create )
{
  dsv_parser_t parser;
  int result = dsv_parser_create(&parser);

  BOOST_REQUIRE_MESSAGE(result == 0,
    "obj_parser_create failed with exit code " << result);

  dsv_parser_destroy(parser);
}

/** \test Create and destroy operations object
 */
BOOST_AUTO_TEST_CASE( operations_create )
{
  dsv_operations_t operations;
  int result = dsv_operations_create(&operations);

  BOOST_REQUIRE_MESSAGE(result == 0,
    "dsv_operations_create failed with exit code " << result);

  dsv_operations_destroy(operations);
}

/** \test Check for proper record callback and context getting and setting
 */
BOOST_AUTO_TEST_CASE( record_callback )
{
  dsv_operations_t operations;
  int result = dsv_operations_create(&operations);

  BOOST_REQUIRE_MESSAGE(result == 0,
    "dsv_operations_create failed with exit code " << result);

  BOOST_REQUIRE_MESSAGE(dsv_get_record_callback(operations) == 0,
    "nonzero record callback on newly initialized operations object " << result);

  BOOST_REQUIRE_MESSAGE(dsv_get_record_context(operations) == 0,
    "nonzero user context on newly initialized operations object");

  detail::record_context context;

  int err = dsv_set_record_callback(detail::record_callback,&context,operations);
  BOOST_REQUIRE_MESSAGE(err == 0,
    "dsv_set_record_callback failed with exit code " << result);

  BOOST_REQUIRE_MESSAGE(dsv_get_record_context(operations) == &context,
    "set record user-defined context does not return the same value");

  BOOST_REQUIRE_MESSAGE(dsv_get_record_callback(operations) == detail::record_callback,
    "set record callback does not return the same value");


  dsv_operations_destroy(operations);
}


BOOST_AUTO_TEST_SUITE_END()

}
}
