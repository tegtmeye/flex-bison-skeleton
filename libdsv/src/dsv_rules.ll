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

lf  \x0A
cr  \x0D

%%

{lf}      {return LF;}
{cr}      {return CR;}
.                   { return yytext[0]; }
%%
