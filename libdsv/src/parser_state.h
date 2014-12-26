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

#ifndef LIBDSV_PARSER_STATE_H
#define LIBDSV_PARSER_STATE_H

#include "dsv_parser.h"

#include <string>
#include <sstream>
#include <list>

#include <boost/shared_ptr.hpp>

namespace detail {

  class file_error : public std::runtime_error {
    public:
      explicit file_error(const std::string &what_arg, int err)
        :std::runtime_error(what_arg), error_num(err) {}

      int error_code(void) const {
        return error_num;
      }

    private:
      int error_num;
  };

  struct parse_operations {
    record_callback_t record_callback;
    void *record_context;
  };



  template<typename CharT>
  struct basic_parse_state {
    typedef typename std::basic_string<CharT> string_type;

    string_type filename;
    string_type dir;
    std::size_t lineno;
    FILE *file;

    basic_parse_state(const std::basic_string<CharT> &fname);
    ~basic_parse_state(void);
  };





  /**
   *  Composition object for dealing with lex/lacc reentrant interface.
   *  ie all extra info gets attached via a void * in lex/yacc
   *
   *  Lifetime is only until the parse completion. That is, do not retain parser
   *  state that only lasts the length of the parse operation
   */
  template<typename CharT>
  class basic_dsv_parser {
    public:
      typedef basic_parse_state<CharT> parser_state_type;

      int parse_file(const char *filename, const parse_operations &operations);

      parser_state_type & state(void) const {
        return state_stack.back()->parser_state;
      }

    private:
      struct state_wrapper;

      std::list<boost::shared_ptr<state_wrapper> > state_stack;
  };


  typedef basic_dsv_parser<char> dsv_parser;

}


#endif
