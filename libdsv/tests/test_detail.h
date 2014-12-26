/*
 Copyright (c) 2014, Mike Tegtmeyer
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors
 may be used to endorse or promote products derived from this software without
 specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *  \file This file is used to provide convenience functions across test cases
 */

#ifndef LIBDSV_TEST_TEST_DETAIL_H
#define LIBDSV_TEST_TEST_DETAIL_H


#ifndef TESTDATA_DIR
#error TESTDATA_DIR not defined
#endif

#define _QUOTEME(x) #x
#define QUOTEME(x) _QUOTEME(x)



#include "parser_state.h"

namespace dsv {
namespace test {

namespace b = boost;
namespace fs = boost::filesystem;


namespace detail {


static const fs::path testdatadir(QUOTEME(TESTDATA_DIR));


struct record_context {
  std::vector<std::string> fields;
  bool should_continue;
};

static int record_callback(const char *fields[], size_t size, void *context)
{
  record_context &check_context = *static_cast<record_context*>(context);

  BOOST_REQUIRE_MESSAGE(size == check_context.fields.size(),
    "record_callback has incorrect number of fields. Should be "
    << check_context.fields.size() << " received " << size);

  for(std::size_t i=0; i<size; ++i) {
    BOOST_REQUIRE_MESSAGE(check_context.fields[i] == fields[i],
      "record_callback saw incorrect field. Should be '"
      << check_context.fields[i] << "' received '" << fields[i] << "'");
  }

  return check_context.should_continue;
}


}


}
}

#endif
