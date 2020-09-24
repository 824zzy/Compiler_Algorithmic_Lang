  // Dalio, Brian A.
  // dalioba
  // 2020-09-17

  //---- DEFINITIONS  --------------------------------------------

%option header-file="lex.yy.h"

/* Our input is not from an interactive source. */
%option never-interactive
/* Don't generate the default rule. */
%option nodefault
/* We don't directly read the input. */
%option noinput
/* Don't include <unistd.h>. */
%option nounistd
/* We don't 'unput' a character back. */
%option nounput
/* Don't call yywrap() on EOF. */
%option noyywrap

/* These two options are doubled to get more detailed reports. */
/* Write performance report to stderr. */
%option perf-report perf-report
/* Write statistics summary to stderr. */
%option verbose verbose

/* Generate warning messages for goofs. */
%option warn

/* Maintain current line number in yylineno. */
%option yylineno

%{
#include <stdio.h>

// Token IDs.  Usually defined by bison.  Since this is a simple
//   flex-only solution, we define them ourselves.
//   The numbers start at 256 to avoid clashing with the literal
//   'single character' token categories.
enum {
  tok_ID = 256,
  tok_INT_LIT,
};

// This union is usually defined by bison.  Since this is a
//   simple flex-only solution, we have to make it ourselves.
union {
  int intval;
} yylval;

// flex doesn't track the column number automatically.  We
//   keep this up-to-date ourselves.
int yycolno;
%}

%% //---- RULES --------------------------------------------------

[0-9]+  {
  yylval.intval = atoi( yytext );

  yycolno += yyleng;
  return tok_INT_LIT;
}

[_a-zA-Z][_a-zA-Z0-9]*  {

  yycolno += yyleng;
  return tok_ID;
}

[ \f\t\v]+  { yycolno += yyleng; }
[\r\n]+     { yycolno = 1; }

.       {
  fprintf( stderr, "(%d:%d) Illegal character '%s'\n",
    yylineno, yycolno, yytext );

    yycolno++;
}

%% //---- USER CODE ----------------------------------------------

int main()
{
  char *inStr = "fred _ 15 1234.345 \"bob\" Maddog87";

  printf( "Tokens for \"%s\":\n", inStr );

  // Initialize the current location.
  //   flex updates yylineno automatically.
  //   We have to keep track of yycolno ourselves.
  yylineno = 1;
  yycolno  = 1;

  yy_scan_string( inStr );

  // Get the tokens one-by-one.
  int tok;
  while ( ( tok = yylex() ) ) {
    printf( "(%d:%d) ", yylineno, yycolno );

    switch ( tok ) {
      case tok_ID:
        printf( "ID '%s'\n", yytext );
        break;

      case tok_INT_LIT:
        printf( "INTEGER_LITERAL %d\n", yylval.intval );
        break;

      default:
        printf( "Unknown token category %d?\n", tok );
        break;
    }
  }

  // No errors!
  return 0;
}

//----------------------------------------------------------------
