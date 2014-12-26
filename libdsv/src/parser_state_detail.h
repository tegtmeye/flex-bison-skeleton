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
 *  \file This file is only here to seperate out uses of the forward-declared
 *  bison functions to avoid circular inclusion problems. ie the Bison parser
 *  needs the contents of \c parser_state.h for the types in
 *  \c dsv_parser_parse() and \c dsv_parser_parse() needs the definition of
 *  \c basic_parser_state and \c parse_operations.
 *
 */

#ifndef LIBDSV_PARSER_STATE_DETAIL_H
#define LIBDSV_PARSER_STATE_DETAIL_H

#include "parser_state.h"

namespace detail {

  template<typename CharT>
  basic_parse_state<CharT>::basic_parse_state(const std::basic_string<CharT> &fname)
    :filename(fname)
  {
    typename string_type::size_type loc = filename.find_last_of('/');
    if(loc == string_type::npos)
      dir = ".";
    else
      dir = filename.substr(0,loc);

    lineno = 1;

    file = fopen(filename.c_str(),"r");
    if(!file)
      throw file_error("Unable to open file",errno);
  }

  template<typename CharT>
  basic_parse_state<CharT>::~basic_parse_state(void)
  {
    // If no filename, then it was created directly from a stream
    if(file && !filename.empty())
      fclose(file);
  }

  // Helper to prevent closing of the stream
//  struct no_close {
//    void operator()(FILE*) {}
//  };

//  template<typename charT, typename ObjContentsT>
//  static int set_stream(FILE *stream, basic_parser_state<charT,ObjContentsT> &state)
//  {
//    typedef basic_parser_state<charT,ObjContentsT> state_type;
//    typedef typename state_type::file_node file_node_type;
//    typedef typename state_type::string_type string_type;
//
//    file_node_type node;
//    node.dir = ".";
//    node.lineno = 1;
//    node.file.reset(stream,no_close());
//    state.file_stack.push_back(node);
//
//    return 0;
//  }



  template<typename CharT>
  struct basic_dsv_parser<CharT>::state_wrapper {
    yyscan_t scanner;
    parser_state_type parser_state;

    state_wrapper(const char *filename);
    ~state_wrapper(void);
  };

  template<typename CharT>
  basic_dsv_parser<CharT>::state_wrapper::state_wrapper(const char *filename)
    :parser_state(filename)
  {
    dsv_parser_lex_init(&scanner);
  }

  template<typename CharT>
  basic_dsv_parser<CharT>::state_wrapper::~state_wrapper(void)
  {
    dsv_parser_lex_destroy(scanner);
  }

  template<typename CharT>
  int basic_dsv_parser<CharT>::parse_file(const char *filename,
    const parse_operations &operations)
  {
    boost::shared_ptr<state_wrapper> new_wrapper(new state_wrapper(filename));
    state_stack.push_back(new_wrapper);

    state_wrapper &wrapper = *state_stack.back();

    dsv_parser_set_in(wrapper.parser_state.file,wrapper.scanner);
    dsv_parser_set_extra(&wrapper.parser_state,wrapper.scanner);

    // start parsing
    int err = dsv_parser_parse(wrapper.scanner,*this,operations);

    state_stack.pop_back();

    if(err == 2)
      err = ENOMEM;

    if(err != 0)
      err = -1;

    return err;
  }

}

#endif
