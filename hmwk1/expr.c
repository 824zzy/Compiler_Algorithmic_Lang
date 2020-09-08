// Dalio, Brian A.
// dalioba
// 2020-09-07

//----------------------------------------------------------
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "expr.h"

char *_exprKindStr[] = {
  "ADD", "DIVIDE", "MULTIPLY","SUBTRACT",
  "INTEGER" };

//----------------------------------------------------------
// Some standard literals.
//   We don't want to be building them all the time, so we
//   allocate static copies of them.
enum _initState { INITIALIZED, NEED_INIT, INITIALIZING }
  _needInit       = NEED_INIT;

Expr *_zeroIntLit = NULL;
Expr *_oneIntLit  = NULL;

static void _initExprLiterals()
{
  // Don't bother initializing unless necessary.
  if ( _needInit != NEED_INIT ) return;

  // Indicates that we are in the middle of initializing.
  _needInit   = INITIALIZING;

  // Some commonly-used literals
  _zeroIntLit = litInteger( 0 );
  _oneIntLit  = litInteger( 1 );

  // All done!
  _needInit   = INITIALIZED;
}

//----------------------------------------------------------
// We keep a linked list of every Expr node that gets allocated
//   so they are easy to clean up later.
static Expr *_exprList;

// Allocate an Expr node -- by default it's a literal integer 0.
//   Returns NULL if the allocation fails.
static Expr *_allocExpr()
{
  _initExprLiterals();

  // Use calloc so the allocated memory is all 0.
  Expr *e = (Expr *) calloc( 1, sizeof( Expr ) );

  // Not an error, just something we want to tell the caller.
  if ( e == NULL ) return NULL;

  // Keep all expr nodes on a list so they are easier to free.
  e->next   = _exprList;
  _exprList = e;

  // Default is a literal -- caller can change this.
  e->kind   = eLIT_INTEGER;

  return e;
}

// Free all Expr nodes that have been allocated so far.
void freeAllExprs()
{
  Expr *e    = _exprList;
  Expr *next = NULL;

  // Loop through all of the expr nodes in the list, freeing
  //   them as we go.  Of course, we have to hold on to next
  //   so we know where to go next.  (Duh.)
  while ( e ) {
    next = e->next;
    free( e );
    e = next;
  }

  // All gone!
  _exprList = NULL;

  // Now that they're gone, get our default literals back.
  _initExprLiterals();
}

//----------------------------------------------------------
// Dump the given Expr node (hierarchically).
static void _dumpExprInternal( Expr *e, int indent, FILE *fp )
{
  switch ( e->kind ) {
    //--------------------------------------
    // Binary operators
    case eBOP_ADD :
    case eBOP_DIVIDE :
    case eBOP_MULTIPLY :
    case eBOP_SUBTRACT :
      fprintf( fp, "%*c(BOP %s\n",
        indent, ' ', _exprKindStr[ e->kind ] );

      _dumpExprInternal( e->left,  indent+1, fp );
      _dumpExprInternal( e->right, indent+1, fp );

      fprintf( fp, "%*c)\n",
        indent, ' ' );

      break;

    //--------------------------------------
    // Literals
    case eLIT_INTEGER :
      fprintf( fp, "%*c(LITERAL %s %d)\n",
        indent, ' ', _exprKindStr[ e->kind ], e->litInteger );

      break;

    //--------------------------------------
    // Don't you kust hate it when something like this happens?
    default :
      fprintf( stderr, "Unknown expression kind %d.\n", e->kind );
      _dumpExprInternal( e, indent, fp );

      break;
  }
}

void dumpExpr( Expr *e )
{
  // Start the indent at 1 unit, output goes to stdout.
  _dumpExprInternal( e, 1, stdout );
}

void dumpExprToFile( Expr *e, FILE *fp )
{
  // Start the indent at 1 unit, output goes to the given file.
  _dumpExprInternal( e, 1, fp );
}

//----------------------------------------------------------
// Evaluate an Expr node, including any hierarchical nodes.
int evalExpr( Expr *e )
{
  switch ( e->kind ) {
    //--------------------------------------
    // Binary operators
    case eBOP_ADD :
      return evalExpr( e->left ) + evalExpr( e->right );

    case eBOP_DIVIDE : {
      int left  = evalExpr( e->left );
      int right = evalExpr( e->right );

      if ( right ) {
        return left / right;
      } else {
        fprintf( stderr, "Attempt to compute %d/0.\n", left );

        // Well, we don't want to just crash out, so return some
        //   kind of useful value:  +/- "infinity" if the
        //   numerator isn't zero also.  On 0 / 0, return 0.
        if ( left ) {
          return left > 0 ? INT_MAX : INT_MIN;
        } else {
          return 0;
        }
      }
    }

    case eBOP_MULTIPLY :
      return evalExpr( e->left ) * evalExpr( e->right );

    case eBOP_SUBTRACT :
      return evalExpr( e->left ) - evalExpr( e->right );

    //--------------------------------------
    // Literals
    case eLIT_INTEGER :
      return e->litInteger;

    //--------------------------------------
    // Don't you just hate it when something like this happens?
    default :
      fprintf( stderr, "Unknown expression kind %d.\n", e->kind );

      return 0;
  }
}

//----------------------------------------------------------
// Build an integer literal Expr node with the given value.
//   Return NULL if the allocation fails.
Expr *litInteger( int value )
{
  Expr *e = _allocExpr();

  if ( e == NULL ) return NULL;

  // _allocExpr already made it a literal, we just have to
  //   set the particular value it's going to be.

  e->litInteger = value;

  return e;
}

//----------------------------------------------------------
// Build a binary operator Expr node with the given left and
//   right operands.
//   Return NULL if the allocation fails.
Expr *oprBinary( ExprKind kind, Expr *left, Expr *right )
{
  Expr *e = _allocExpr();

  if ( e == NULL ) return NULL;

  // _allocExpr made it a literal.  We change its kind to be
  //   whatever binary operator it is and give it the left and
  //   right operands.

  e->kind  = kind;

  e->left  = left;
  e->right = right;

  return e;
}

//----------------------------------------------------------
