/* Dalio, Brian A. */
/* dalioba         */
/* 2020-09-07      */

   /**** DEFINITIONS ********************************************/
%{
#include "expr.h"

#include "parser.tab.h"
#include "lex.yy.h"

void yyerror( Expr **, void *, const char * );

%}

%code requires {
  typedef void *yyscan_t;
}

%define api.pure full

%defines

%lex-param   { yyscan_t scanner }
%parse-param { Expr **expr }
%parse-param { yyscan_t scanner }

  /* Value stack type */
%union {
  int litInteger;
  Expr *expr;
}

  /* Token names (and types, where necessary) */
%token TOKEN_MINUS
%token TOKEN_PLUS
%token TOKEN_SLASH
%token TOKEN_STAR

%token TOKEN_LPAREN
%token TOKEN_RPAREN

%token <litInteger> TOKEN_INTEGER

  /* Nonterminal types */
%type <expr> expr term factor

%% /**** RULES **************************************************/

start
  : expr                    { *expr = $1; }
  ;

expr
  : term                    { $$ = $1; }
  | expr TOKEN_MINUS term   { $$ = oprBinary( eBOP_SUBTRACT, $1, $3 ); }
  | expr TOKEN_PLUS  term   { $$ = oprBinary( eBOP_ADD,      $1, $3 ); }
  ;

term
  : factor                  { $$ = $1; }
  | term TOKEN_SLASH factor { $$ = oprBinary( eBOP_DIVIDE,   $1, $3 ); }
  | term TOKEN_STAR  factor { $$ = oprBinary( eBOP_MULTIPLY, $1, $3 ); }
  ;

factor
  : TOKEN_INTEGER           { $$ = litInteger( $1 ); }
  | TOKEN_LPAREN expr TOKEN_RPAREN  { $$ = $2; }
  ;

%% /**** USER CODE **********************************************/

void yyerror( Expr **_expr, void *_scanner, const char *msg )
{
  /* Unused at present */ (void) _expr; (void) _scanner;

  fprintf( stderr, "Error: %s\n", msg );
}
