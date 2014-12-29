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

typedef void* yyscan_t;
#include "dsv_grammar.hh"
#include "dsv_rules.h"

#include "dsv_parser.h"
#include "parser_state_detail.h"

#include <errno.h>
#include <stdlib.h>




namespace detail {


}


extern "C" {

  int dsv_parser_create(dsv_parser_t *parser)
  {
    int err = 0;

    try {
      parser->p = new detail::dsv_parser;
    }
    catch (std::bad_alloc &) {
      err = ENOMEM;
    }
    catch (...) {
      abort();
    }

    return err;
  }

  void dsv_parser_destroy(dsv_parser_t parser)
  {
    try {
      delete static_cast<detail::dsv_parser*>(parser.p);
    }
    catch(...) {
      abort();
    }
  }



  int dsv_operations_create(dsv_operations_t *_operations)
  {
    int err = 0;

    try {
      detail::parse_operations *operations = new detail::parse_operations;
      operations->record_callback = 0;
      operations->record_context = 0;

      _operations->p = operations;

    }
    catch (std::bad_alloc &) {
      err = ENOMEM;
    }
    catch (...) {
      abort();
    }

    return err;
  }

  void dsv_operations_destroy(dsv_operations_t operations)
  {
    try {
      delete static_cast<detail::parse_operations*>(operations.p);
    }
    catch(...) {
      abort();
    }
  }

  record_callback_t dsv_get_record_callback(dsv_operations_t _operations)
  {
    if(_operations.p == 0)
      return 0;

    detail::parse_operations &operations =
      *static_cast<detail::parse_operations*>(_operations.p);

    record_callback_t result = 0;

    try {
      result = operations.record_callback;

    }
    catch(...) {
      abort();
    }

    return result;
  }

  void * dsv_get_record_context(dsv_operations_t _operations)
  {
    if(_operations.p == 0)
      return 0;

    detail::parse_operations &operations =
      *static_cast<detail::parse_operations*>(_operations.p);

    void * result = 0;

    try {
      result = operations.record_context;

    }
    catch(...) {
      abort();
    }

    return result;
  }



  int dsv_set_record_callback(record_callback_t fn, void *context,
    dsv_operations_t _operations)
  {
    if(_operations.p == 0)
      return EINVAL;

    detail::parse_operations &operations =
      *static_cast<detail::parse_operations*>(_operations.p);

    int err = 0;

    try {
      operations.record_callback = fn;
      operations.record_context = context;
    }
    catch(...) {
      abort();
    }

    return err;
  }

  int dsv_parse(const char *filename, dsv_parser_t _parser,
                dsv_operations_t _operations)
  {
    if(_parser.p == 0 || _operations.p == 0)
      return EINVAL;

    detail::dsv_parser &parser = *static_cast<detail::dsv_parser*>(_parser.p);
    detail::parse_operations &operations = *static_cast<detail::parse_operations*>(_operations.p);

    int err = 0;

    try {
      err = parser.parse_file(filename,operations);
    }
    catch(std::bad_alloc &) {
      err = ENOMEM;
    }
    catch(std::exception &ex) {
      err = -1;
    }
    catch(...) {
      abort();
    }

    return err;
  }

  size_t dsv_parse_error(dsv_parser_t _parser, dsv_log_level log_level, char *buf, size_t len)
  {
    if(_parser.p == 0)
      return 0;

    detail::dsv_parser &parser = *static_cast<detail::dsv_parser*>(_parser.p);

    size_t result = 0;

    try {
      // add the null terminator
      result = 1;
      if(buf && len)
        *buf = 0;

      size_t buf_len = (len>0?len-1:0);
      detail::dsv_parser::const_msg_iterator cur = parser.msg_begin();
      while(cur != parser.msg_end()) {
        if(cur->first & log_level) {
          std::size_t msg_size = cur->second.size();
          std::size_t copy_size = std::min(msg_size,buf_len);
          if(buf && buf_len) {
            buf = std::copy(cur->second.begin(),cur->second.begin()+copy_size,buf);
            *buf = 0;
          }

          result += msg_size;
          buf_len -= copy_size;
        }
        ++cur;
      }
    }
    catch(std::exception &ex) {
      result = 0;
    }
    catch(...) {
      abort();
    }

    return result;
  }



}
