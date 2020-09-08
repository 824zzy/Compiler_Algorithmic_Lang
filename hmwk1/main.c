// Dalio, Brian A.
// dalioba
// 2020-09-07

//----------------------------------------------------------
#include <errno.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <string.h>

#include "expr.h"

#include "parser.tab.h"
#include "lex.yy.h"

//----------------------------------------------------------
int parseString( const char *exprStr, Expr **expr )
{
  YY_BUFFER_STATE  bufferState;
  int              parseState;
  yyscan_t         scanner;

  // Initialize the Lexical Analyzer (the scanner).
  if ( yylex_init( &scanner ) ) {
    fprintf( stderr,
      "Unable to initialize the lexical analyzer.  Error is (%d) \"%s\".\n",
      errno, strerror( errno ) );

    // We have to signal to the caller that we didn't even
    //   get to the parser so there's no expression.
    *expr = NULL;

    return -1;
  }

  // Set the given string as the input for the scanner to scan.
  bufferState = yy_scan_string( exprStr, scanner );

  // Using the established scanner as a source of tokens, parse
  //   the input.  The returned state tells us whether it was a
  //   success or not.
  parseState = yyparse( expr, scanner );

  // Now that the parse is over, dispose of the scanner info.
  yy_delete_buffer( bufferState, scanner );
  yylex_destroy( scanner );

  // Return the state we got back from parsing.
  return parseState;
}

//----------------------------------------------------------
int main()
{
  char *buffer = NULL;
  char *text;

  Expr *expr;
  int   parseState;
  int   result;

  // Get the readline history set up so we can get back previous
  //   strings.
  using_history();

  while ( 1 ) {
    // Prompt the user for some input.  We're using the GNU
    //   readline routine so we get more comfortable processing,
    //   editing, history, etc.
    buffer = readline( "> " );

    // Oops, some error or maybe EOF.
    if ( buffer == NULL ) break;

    // Skip over leading whitespace.  If nothing remaining, get
    //   another line of input (after freeing the buffer readline
    //   returned).  If there is a string, add it to the history.
    text = buffer;
    while ( isspace( *text ) ) text++;
    if ( *text == '\0' ) {
      free( buffer );
      continue;
    }
    add_history( text );

    // Parse the string!
    parseState = parseString( text, &expr );
    if ( parseState ) {
      // Didn't parse --
      //   Inform the user what happened.
      fprintf( stderr,
        "Unable to parse \"%s\".  Error is (%d) \"%s\".\n",
        text,
        parseState,
        parseState == -1 ? "Unable to initialize lexical analyzer" :
        parseState ==  1 ? "Invalid input" :
        parseState ==  2 ? "Out of memory" : "Unknown error?" );
    } else {
      // Did parse --
      //   Evaluate the expression and announce results.
      result = evalExpr( expr );
      printf( "\"%s\" ==> %d.\n\n", text, result );
      dumpExpr( expr );
    }

    // Deallocate the line and all expression nodes ...
    free( buffer );
    freeAllExprs();
  }

  return 0;
}

//----------------------------------------------------------
