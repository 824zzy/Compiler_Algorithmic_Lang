/* Dalio, Brian A. */
/* dalioba         */
/* 2020-08-28      */

   /**** DEFINITIONS ********************************************/
%{
#include <stdio.h>

#include "expr.h"

#include "parser.tab.h"
%}

%option bison-bridge

%option header-file="lex.yy.h"

%option never-interactive

%option nodefault
%option noinput
%option nounistd
%option nounput
%option noyywrap

%option reentrant

%option verbose verbose

%option warn

%option yylineno

%% /**** RULES **************************************************/

"-"     { return TOKEN_MINUS; }
"+"     { return TOKEN_PLUS; }
"/"     { return TOKEN_SLASH; }
"*"     { return TOKEN_STAR; }

"("     { return TOKEN_LPAREN; }
")"     { return TOKEN_RPAREN; }

[0-9]+  {
  sscanf( yytext, "%d", &yylval->litInteger );
  return TOKEN_INTEGER;
}


        /* Ignore extraneous whitespace. */
[ \r\n\t]*  { continue; }

        /* For now, ignore any other characters. */
.       { continue; }

%% /**** USER CODE **********************************************/

/* Nothing so far ... */