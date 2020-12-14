#ifndef __ANSIN_H
#define __ANSIN_H

#define SUCCES 1
extern int idxCrtAtom;

int factor();
int exprPrefix();
int exprMul();
int exprAdd();
int exprComp();
int exprAssign();
int exprLogic();
int expr();
int instr();
int funcParam();
int funcParams();
int block();
int defFunc();
int baseType();
int defVar();
int program();

#endif //__ANSIN_H