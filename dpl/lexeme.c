/*
cs403 Shengting Cao
Programming Language
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexeme.h"
#include "type.h"

Lexeme *newLexeme(char *type)
{
    Lexeme *L = malloc(sizeof(Lexeme));
    L->type = type;
    L->stringVal = NULL;
    L->intVal = 0;
    L->left = NULL;
    L->right = NULL;
    L->array = NULL;
    return L;
}

Lexeme *cons(char *t, Lexeme *l, Lexeme *r)
{
    Lexeme *L = malloc(sizeof(Lexeme));
    L->type = t;
    L->left = l;
    L->right = r;
    return L;
}

Lexeme *car(Lexeme *L)
{
    return L->left;
}

Lexeme *cdr(Lexeme *L)
{
    return L->right;
}

void setCar(Lexeme *L, Lexeme *l)
{
    L->left = l;
}

void setCdr(Lexeme *L, Lexeme *r)
{
    L->right = r;
}

char *getType(Lexeme *L)
{
    return L->type;
}

char *showLexeme(Lexeme *L)
{
    char *display = malloc(sizeof(char) * 128);
    if (L->type == STRING || L->type == IDENTIFIER)
    {
        sprintf(display, "%s %s", L->type, L->stringVal);
    }
    else if (L->type == INTEGER)
    {
        strcpy(display, L->type);
        char buf[10];
        sprintf(buf, "%d", L->intVal);
        strcat(display, " ");
        strcat(display, buf);
    }
    else
    {
        strcpy(display, L->type);
    }
    return display;
}
