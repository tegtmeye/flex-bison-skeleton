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

%code requires {
  /*
    Forward declarations for forward declarations of bison types
  */
  typedef void* yyscan_t;

  #include "parser_state.h"

}


%code {
  /*
  This is needed because the flex-bison bridge is broke as of this writing.
  That is, the generated rules.h file requires YYSTYPE to be defined and
  grammar.hh requires yyscan_t to be defined. So there is no combination of
  includes that will not give you a compiler error.
  */
  typedef void* yyscan_t;
  #include "dsv_grammar.hh"
  #include "dsv_rules.h"

  #include "parser_state_detail.h"


//   void dsv_parser_error(yyscan_t scanner, detail::dsv_parser &parser,
//     const detail::parse_operations &operations, const char *s);

  /**
   *  Error reporting function as required by yacc
   */
  void dsv_parser_error(yyscan_t scanner, detail::dsv_parser &parser,
    const detail::parse_operations &operations, const char *s)
  {
    std::cerr << "HERE!!!!!!!!'" << s << "'\n";
  }

  /**
   *  Use namespaces here to avoid multiple symbol name clashes
   */
  namespace dsv {
    /**
     *  convenience routines
     */
//    inline static dsv::parser_extra & get_state(yyscan_t scanner) {
//      return *static_cast<parser_extra*>(dsv_parser_get_extra(scanner));
//    }

  }

}

%pure-parser

%union {
  float real;
  int integer;
  int reference[3];
  bool toggle;
}

%lex-param {yyscan_t scanner}
%parse-param {yyscan_t scanner}
%parse-param {detail::dsv_parser &parser}
%parse-param {const detail::parse_operations &operations}

%token CR
%token LF




%% /* The grammar follows.  */

input:
  /* empty */
  | input record
  ;


record:
  newline
  ;

newline
  : LF
    {
      if(parser.newline_behavior() == dsv_newline_crlf_strict) {
        parser.push_msg("LFSeenOnCRLFStrict",dsv_log_error);
        YYABORT;
      }
    }
  | CR LF
    {
      if(parser.newline_behavior() == dsv_newline_lf_strict) {
        parser.push_msg("CRSeenOnLFStrict",dsv_log_error);
        YYABORT;
      }
    }

%%
