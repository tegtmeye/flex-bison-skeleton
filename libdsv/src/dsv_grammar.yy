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


  void dsv_parser_error(yyscan_t scanner, detail::dsv_parser &parser,
    const detail::parse_operations &operations, const char *s);

  /**
   *  Error reporting function as required by yacc
   */
  void dsv_parser_error(yyscan_t scanner, detail::dsv_parser &parser,
    const detail::parse_operations &operations, const char *s)
  {
//    detail::verbose_output_formatter(detail::get_state(scanner),s);
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


%%

newline: '\n'

%%
