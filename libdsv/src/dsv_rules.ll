%{
  #ifdef HAVE_CONFIG_H
  #include <config.h>
  #endif

  #include "parser_state.h"

  #include <sys/types.h>

  #include "dsv_grammar.hh"

  #include <stdlib.h>
  #include <limits.h>

  #if NO_POSIX_READ
  #define YY_INPUT(buf,result,max_size) \
    errno=0; \
    while ( (result = fread(buf, 1, max_size, yyin))==0 && ferror(yyin)) { \
      if( errno != EINTR) { \
        YY_FATAL_ERROR( "input in flex scanner failed" ); \
        break; \
      } \
      errno=0; \
      clearerr(yyin); \
    }
  #endif

%}

/* Options section */
%option full never-interactive
%option warn nodefault noyywrap nounput
%option reentrant bison-bridge

/* State Definitions Section */


/* Definitions Section */



%%

.                   { return yytext[0]; }
%%
