/*
cs403 Shengting Cao
Programming Language
*/
#ifndef ENV_H
#define ENV_H
#include <stdio.h>
#include "lexeme.h"

extern Lexeme *createEnv();
extern Lexeme *extendEnv(Lexeme *env, Lexeme *vars, Lexeme *vals);
extern Lexeme *makeTable(Lexeme *vars, Lexeme *vals);
extern Lexeme *lookupEnv(Lexeme *var, Lexeme *env);
extern int sameVariable(Lexeme *x, Lexeme *y);
extern Lexeme *insert(Lexeme *var, Lexeme *val, Lexeme *env);
extern Lexeme *updateEnv(Lexeme *var, Lexeme *env, Lexeme *newVariable);
#endif
