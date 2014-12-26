#include <boost/test/unit_test.hpp>

#include <dsv_parser.h>


#include <errno.h>
#include <stdio.h>


/** \file
 *  \brief Unit tests for creation
 */

namespace dsv {
namespace test {


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
BOOST_AUTO_TEST_CASE( operataions_create )
{
  dsv_operations_t operations;
  int result = dsv_operations_create(&operations);

  BOOST_REQUIRE_MESSAGE(result == 0,
    "dsv_operations_create failed with exit code " << result);

  dsv_operations_destroy(operations);
}

BOOST_AUTO_TEST_SUITE_END()

}
}
