/*
cs403 Shengting Cao
Programming Language
*/
#include <stdio.h>
#include "environment.h"
#include "lexeme.h"
#include "type.h"

Lexeme *createEnv()
{
    return extendEnv(NULL, NULL, NULL);
}

Lexeme *extendEnv(Lexeme *env, Lexeme *vars, Lexeme *vals)
{
    return cons(ENVIRONMENT, makeTable(vars, vals), env);
}

Lexeme *makeTable(Lexeme *vars, Lexeme *vals)
{
    return cons(TABLE, vars, vals);
}

Lexeme *lookupEnv(Lexeme *var, Lexeme *env)
{
    while (env != NULL)
    {
        Lexeme *table = car(env);
        Lexeme *vars = car(table);
        Lexeme *vals = cdr(table);
        while (vars != NULL)
        {
            if (sameVariable(var, car(vars)))
            {
                return car(vals);
            }
            //walk the lists in parallel
            vars = cdr(vars);
            vals = cdr(vals);
        }
        env = cdr(env);
    }
    fprintf(stderr, "FATAL: variable, %s, is undefined.", var->stringVal);
    return NULL;
}

int sameVariable(Lexeme *x, Lexeme *y)
{
    return !strcmp(x->stringVal, y->stringVal);
}

Lexeme *insert(Lexeme *var, Lexeme *val, Lexeme *env)
{
    Lexeme *table = car(env);
    setCar(table, cons(GLUE, var, car(table)));
    setCdr(table, cons(GLUE, val, cdr(table)));
    return val;
}

Lexeme *updateEnv(Lexeme *var, Lexeme *val, Lexeme *env)
{
     while (env != NULL)
    {
        Lexeme *table = car(env);
        Lexeme *vars = car(table);
        Lexeme *vals = cdr(table);
        while (vars != NULL)
        {
            if (sameVariable(var, car(vars)))
            {   
                setCar(var, val);
                return val;
            }
            //walk the lists in parallel
            vars = cdr(vars);
            vals = cdr(vals);
        }
        env = cdr(env);
    }
    fprintf(stderr, "FATAL: variable, %s, is undefined.", var->stringVal);
    return NULL;
}