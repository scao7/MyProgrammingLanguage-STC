/*
cs403 Shengting Cao
Programming Language
*/
#ifndef LEXEME_H
#define LEXEME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct lexeme
{
    char *type;

    char *stringVal;
    int intVal;
    FILE *file;
    struct lexeme *(*fval)(struct lexeme *);
    struct lexeme *left;
    struct lexeme *right;

    struct lexeme **array;

} Lexeme;

extern Lexeme *newLexeme(char *type);

extern Lexeme *cons(char *t, Lexeme *l, Lexeme *r);
extern Lexeme *car(Lexeme *x);
extern Lexeme *cdr(Lexeme *x);
extern void setCar(Lexeme *x, Lexeme *l);
extern void setCdr(Lexeme *x, Lexeme *l);
extern char *getType(Lexeme *x);
extern char *showLexeme(Lexeme *x);

#endif
