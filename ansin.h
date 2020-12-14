#ifndef __ANSIN_H
#define __ANSIN_H
#include <stdint-gcc.h>

static const int SUCCES = 1;
static const int FAILURE = 0;
extern int idxCrtAtom;

void err_msg(char *s);

// program :: = (defVar | defFunc | block) * FINISH int program();
int program();
//defVar ::= VAR ID COLON baseType SEMICOLON
int defVar();
// baseType :: = TYPE_INT | TYPE_REAL | TYPE_STR
int baseType();
// defFunc :: = FUNCTION ID LPAR funcParams RPAR COLON baseType defVar * block END
int defFunc();
// block :: = instr +
int block();
// funcParams :: = (funcParam(COMMA funcParam) *) ?
int funcParams();
// funcParam :: = ID COLON baseType
int funcParam();
// instr :: = expr ? SEMICOLON
//          | IF LPAR expr RPAR block(ELSE block) ? END
//          | RETURN expr SEMICOLON
//          | WHILE LPAR expr RPAR block END
int instr();
// expr :: = exprLogic
int expr();
// exprLogic :: = exprAssign((AND | OR) exprAssign) *
int exprLogic();
// exprAssign :: = (ID ASSIGN) ? exprComp
int exprAssign();
// exprComp :: = exprAdd((LESS | EQUAL) exprAdd) ?
int exprComp();
// exprAdd :: = exprMul((ADD | SUB) exprMul) *
int exprAdd();
// exprMul :: = exprPrefix((MUL | DIV) exprPrefix) *
int exprMul();
// exprPrefix :: = (SUB | NOT) ? factor
int exprPrefix();
// factor ::= INT
//          | REAL
//          | STR
//          | LPAR expr RPAR
//          | ID ( LPAR ( expr ( COMMA expr )* )? RPAR )?
int factor();

#endif //__ANSIN_H