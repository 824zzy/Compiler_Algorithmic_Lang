// Dalio, Brian A.
// dalioba
// 2020-09-07

#if !defined( __EXPR_H__ )
#define __EXPR_H__

#include <stdio.h>

//----------------------------------------------------------
// The different kinds of things that an expression can be.
typedef enum _ExprKind
{
  eBOP_ADD,
  eBOP_DIVIDE,
  eBOP_MULTIPLY,
  eBOP_SUBTRACT,

  eLIT_INTEGER,
} ExprKind;

typedef struct _Expr
{
  struct _Expr *next;

  ExprKind kind;

  int litInteger;

  struct _Expr *left;
  struct _Expr *right;
} Expr;

//----------------------------------------------------------
void dumpExpr( Expr *e );
void dumpExprToFile( Expr *e, FILE *fp );
int  evalExpr( Expr *e );
void freeAllExprs();

Expr *litInteger( int value );
Expr *oprBinary( ExprKind kind, Expr *left, Expr *right );

//----------------------------------------------------------
#endif
