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
#include <vector>
#include <utility>

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
    private:
      typedef std::vector<std::pair<dsv_log_level,std::string> > msg_stack_type;

    public:
      typedef basic_parse_state<CharT> parser_state_type;

      typedef typename msg_stack_type::const_iterator const_msg_iterator;

      basic_dsv_parser(void);

      dsv_newline_behavior newline_behavior(void) const;
      dsv_newline_behavior newline_behavior(dsv_newline_behavior behavior);


      int parse_file(const char *filename, const parse_operations &operations);

      parser_state_type & state(void) const {
        return state_stack.back()->parser_state;
      }

      void push_msg(const std::string &what, dsv_log_level log_level);

      const_msg_iterator msg_begin(void) const;
      const_msg_iterator msg_end(void) const;

    private:
      struct state_wrapper;
      typedef std::list<boost::shared_ptr<state_wrapper> > state_stack_type;

      std::string localized_include(void);

      dsv_newline_behavior newline_flag;

      state_stack_type state_stack;

      msg_stack_type msg_vec;
  };

  template<typename CharT>
  inline basic_dsv_parser<CharT>::basic_dsv_parser(void)
    :newline_flag(dsv_newline_permissive)
  {
  }

  template<typename CharT>
  inline dsv_newline_behavior basic_dsv_parser<CharT>::newline_behavior(void) const
  {
    return newline_flag;
  }

  template<typename CharT>
  inline dsv_newline_behavior
  basic_dsv_parser<CharT>::newline_behavior(dsv_newline_behavior behavior)
  {
    dsv_newline_behavior tmp = newline_flag;
    newline_flag = behavior;
    return tmp;
  }

  template<typename CharT>
  void basic_dsv_parser<CharT>::push_msg(const std::string &what, dsv_log_level log_level)
  {
    std::stringstream msg;

    msg << localized_include() << "\n";

    // need to localize this
    msg << what;

    msg_vec.push_back(std::make_pair(log_level,msg.str()));
  }

  template<typename CharT>
  typename basic_dsv_parser<CharT>::const_msg_iterator
  basic_dsv_parser<CharT>::msg_begin(void) const
  {
    return msg_vec.begin();
  }

  template<typename CharT>
  typename basic_dsv_parser<CharT>::const_msg_iterator
  basic_dsv_parser<CharT>::msg_end(void) const
  {
    return msg_vec.end();
  }


  template<typename CharT>
  std::string basic_dsv_parser<CharT>::localized_include(void)
  {
    std::stringstream out;

    typename state_stack_type::const_reverse_iterator rcur = state_stack.rbegin();
    assert(rcur != state_stack.rend());

    out << "In file: '" << (*rcur)->parser_state.filename << "'";

    for(++rcur; rcur != state_stack.rend(); ++rcur)
      out << "\n\tIncluded from: '" << (*rcur)->parser_state.filename << "'";

    return out.str();
  }

  typedef detail::basic_dsv_parser<char> dsv_parser;

}


#endif
