/*
cs403 Shengting Cao
Programming Language
*/
#include <stdio.h>
#include "eval.h"
#include "lexeme.h"
#include "type.h"
#include "environment.h"
#include "type.h"
// #include "Fatal.h"

Lexeme *evalFuncDef(Lexeme *t, Lexeme *env)
{
    Lexeme *closure = newLexeme(CLOSURE);
    setCar(closure, env);
    setCdr(closure, t);
    return insert(getFuncDefName(t), closure, env);
}

Lexeme *evalFuncCall(Lexeme *t, Lexeme *env)
{
    Lexeme *closure = eval(getFuncCallName(t), env);
    Lexeme *args = getFuncCallArgs(t);
    Lexeme *eargs = evalArgs(args, env);
    if (!strcmp(closure->type, BUILTIN))
    {
        return closure->fval(eargs);
    }

    Lexeme *params = getClosureParams(closure);
    Lexeme *body = getClosureBody(closure);
    Lexeme *senv = getClosureEnvironment(closure);
    Lexeme *xenv = extendEnv(senv, params, eargs);

    // Lexeme *n = newLexeme(IDENTIFIER);
    // n->stringVal = "this";
    // insert(n,xenv,xenv);
    return eval(body, xenv);
}

Lexeme *evalArgs(Lexeme *t, Lexeme *env)
{
    if (t == NULL)
    {
        return NULL;
    }
    return cons(GLUE, eval(car(t), env), evalArgs(cdr(t), env));
}

Lexeme *evalBlock(Lexeme *t, Lexeme *env)
{
    Lexeme *result;
    while (t != NULL)
    {
        result = eval(car(t), env);
        if (result != NULL && !strcmp(result->type, BREAK))
        {
            return result;
        }
        t = cdr(t);
    }
    return result;
}

Lexeme *evalDot(Lexeme *t, Lexeme *env)
{
    // printf("%s", t->left->type);
    Lexeme *l = eval(t->left, env);

    Lexeme *r = lookupEnv(t->right, l);
    // Lexeme* r = eval(t->right,l);
    return r;
}

Lexeme *evalPlus(Lexeme *t, Lexeme *env)
{
    //eval the left and the right hand sides
    Lexeme *left = eval(car(t), env);
    Lexeme *right = eval(cdr(t), env);
    if (left->type == INTEGER && right->type == INTEGER)
    {
        Lexeme *new = newLexeme(INTEGER);
        new->intVal = left->intVal + right->intVal;
        return new;
    }
    else if (left->type == INTEGER && right->type == STRING)
    {
        Lexeme *new = newLexeme(STRING);
        sprintf(new->stringVal, "%d%s", left->intVal, right->stringVal);
        return new;
    }
    else if (left->type == STRING && right->type == INTEGER)
    {
        Lexeme *new = newLexeme(STRING);
        sprintf(new->stringVal, "%s%d", left->stringVal, right->intVal);
        return new;
    }
    else if (left->type == STRING && right->type == STRING)
    {
        Lexeme *new = newLexeme(STRING);
        sprintf(new->stringVal, "%s%s", left->stringVal, right->stringVal);
        return new;
    }
    else
    {
        Fatal("Can only add ints or strings.");
        exit(1);
        return NULL;
    }
}

Lexeme *evalMinus(Lexeme *t, Lexeme *env)
{
    //eval the left and the right hand sides
    Lexeme *left = eval(car(t), env);
    Lexeme *right = eval(cdr(t), env);
    if (left->type == INTEGER && right->type == INTEGER)
    {
        Lexeme *new = newLexeme(INTEGER);
        new->intVal = left->intVal - right->intVal;
        return new;
    }
    else
    {
        Fatal("FATAL: Can only subtract INTEGER.");
        exit(1);
        return NULL;
    }
}

Lexeme *evalDivide(Lexeme *t, Lexeme *env)
{
    //eval the left and the right hand sides
    Lexeme *left = eval(car(t), env);
    Lexeme *right = eval(cdr(t), env);
    if (left->type == INTEGER && right->type == INTEGER)
    {
        Lexeme *new = newLexeme(INTEGER);
        new->intVal = left->intVal / right->intVal;
        return new;
    }
    else
    {
        Fatal("Can only divide INTEGER.");
        exit(1);
        return NULL;
    }
}

Lexeme *evalTimes(Lexeme *t, Lexeme *env)
{
    //eval the left and the right hand sides
    Lexeme *left = eval(car(t), env);
    Lexeme *right = eval(cdr(t), env);
    if (left->type == INTEGER && right->type == INTEGER)
    {
        Lexeme *new = newLexeme(INTEGER);
        new->intVal = left->intVal * right->intVal;
        return new;
    }
    else
    {
        Fatal("Can only times INTEGER.");
        exit(1);
        return NULL;
    }
}

Lexeme *evalExponent(Lexeme *t, Lexeme *env)
{
    //eval the left and the right hand sides
    Lexeme *left = eval(car(t), env);
    Lexeme *right = eval(cdr(t), env);
    if (left->type == INTEGER && right->type == INTEGER)
    {
        Lexeme *new = newLexeme(INTEGER);
        int i, result = 1;
        for (i = 0; i < right->intVal; i++)
        {
            result *= left->intVal;
        }
        new->intVal = result;
        return new;
    }
    else
    {
        Fatal("Can only TIMES INTEGER.");
        exit(1);
        return NULL;
    }
}

Lexeme *evalAssign(Lexeme *t, Lexeme *env)
{
    // printf("%s", t->type);
    Lexeme *value = eval(cdr(t), env);

    if (!strcmp(t->left->type, ARRAYACCESS))
    {
        Lexeme *temp = evalArrayAccess(t->left, env);
        temp->type = value->type;
        temp->stringVal = value->stringVal;
        temp->intVal = value->intVal;
        temp->fval = value->fval;
        temp->left = value->left;
        temp->right = value->right;
        temp->array = value->array;
    }
  
    // else if (!strcmp(t->left->type,IDENTIFIER))
    // {   printf("%s",car(t)->type);
    //     updateEnv(t->left,value,env);
    // }
    // else if (!strcmp(t->left->type,DOT))
    // {
    //     printf("%s",car(t)->type);
    //     Lexeme *object = eval(car(car(t)), env);
    //     updateEnv(car(cdr(t)),value,object);
    // }
    else
    {
        insert(car(t), value, env);
    }
    return value;
}

Lexeme *evalWhile(Lexeme *t, Lexeme *env)
{
    Lexeme *result = NULL;
    while (isTrue(eval(car(t), env)))
    {
        result = eval(cdr(t), env);
        if (!strcmp(result->type, BREAK))
        {
            return newLexeme(NIL);
        }
    }
    return result;
}

Lexeme *evalIf(Lexeme *t, Lexeme *env)
{
    Lexeme *result = NULL;
    if (isTrue(eval(t->left, env)))
    {
        result = eval(t->right->left, env);
    }
    else
    {
        result = eval(t->right->right, env);
    }
    return result;
}

Lexeme *evalOptElse(Lexeme *t, Lexeme *env)
{
    if (t == NULL)
    {
        return NULL;
    }
    else
    {
        return eval(t->left, env);
    }
}

Lexeme *evalFor(Lexeme *t, Lexeme *env)
{
    Lexeme *result = NULL;
    eval(car(car(t)), env);
    while (isTrue(eval(cdr(car(t)), env)))
    {
        result = eval(t->right->right, env);
        eval(t->right->left, env);
    }
    return result;
}

Lexeme *evalExpressionList(Lexeme *t, Lexeme *env)
{
    Lexeme *result = newLexeme(NIL);
    while (t != NULL)
    {
        result = eval(t->left, env);
        if (result != NULL && !strcmp(result->type, BREAK))
        {
            return result;
        }
        t = t->right;
    }
    return result;
}

Lexeme *evalExpr(Lexeme *t, Lexeme *env)
{
    if (t->right == NULL)
    {
        return eval(t->left, env);
    }
    else
    {
        return evalSimpleOp(cons(t->right->left->type, t->left, t->right->right), env);
    }
}

Lexeme *evalLambda(Lexeme *t, Lexeme *env)
{
    Lexeme *closure = newLexeme(CLOSURE);
    Lexeme *temp = newLexeme(FUNCDEF);
    temp->right = t;
    setCar(closure, env);
    setCdr(closure, temp);
    return closure;
}

Lexeme *evalSimpleOp(Lexeme *t, Lexeme *env)
{
    t->right = eval(t->right, env);
    if (!strcmp(t->type, EQUALS))
    {
        return evalAssign(t, env);
    }
    t->left = eval(t->left, env);
    Lexeme *result = newLexeme(INTEGER);
    if (!strcmp(t->type, NOT))
    {
    }
    else if (!strcmp(t->type, NOTEQUAL))
    {
        if (!strcmp(t->left->type, INTEGER) && !strcmp(t->right->type, INTEGER))
        {
            if (t->left->intVal == t->right->intVal)
            {
                result->intVal = 0;
            }
            else
            {
                result->intVal = 1;
            }
        }
        else if ((!strcmp(t->left->type, STRING) || !strcmp(t->left->type, IDENTIFIER)) && (!strcmp(t->right->type, STRING) || !strcmp(t->left->type, IDENTIFIER)))
        {
            result->intVal = !strcmp(t->left->stringVal, t->right->stringVal);
        }
        else if (!strcmp(t->left->type, NIL) && !strcmp(t->right->type, STRING))
        {
            result->intVal = 1;
        }
        else if (!strcmp(t->left->type, STRING) && !strcmp(t->right->type, NIL))
        {
            result->intVal = 1;
        }
        else if (!strcmp(t->left->type, NIL) && !strcmp(t->right->type, INTEGER))
        {
            result->intVal = 1;
        }
        else if (!strcmp(t->left->type, INTEGER) && !strcmp(t->right->type, NIL))
        {
            result->intVal = 1;
        }
        else if (!strcmp(t->left->type, STRING) && !strcmp(t->right->type, INTEGER))
        {
            result->intVal = 1;
        }
        else if (!strcmp(t->left->type, INTEGER) && !strcmp(t->right->type, STRING))
        {
            result->intVal = 1;
        }
        else if (!strcmp(t->left->type, NIL) && !strcmp(t->right->type, NIL))
        {
            result->intVal = 0;
        }
        else if (!strcmp(t->left->type, NIL) && strcmp(t->right->type, NIL))
        {
            result->intVal = 1;
        }
        else if (strcmp(t->left->type, NIL) && !strcmp(t->right->type, NIL))
        {
            result->intVal = 1;
        }
        else
        {
            printf("l->%s, r->%s\n", t->left->type, t->right->type);
            Fatal("compare error.");
            exit(1);
            return NULL;
        }
    }
    else if (!strcmp(t->type, GREATERTHAN))
    {
        if (!strcmp(t->left->type, INTEGER) && !strcmp(t->right->type, INTEGER))
        {
            if (t->left->intVal > t->right->intVal)
            {
                result->intVal = 1;
            }
            else
            {
                result->intVal = 0;
            }
        }
        else
        {
            Fatal("need compare with integer.");
            printf("Types error: %s %s\n", t->left->type, t->right->type);
            exit(1);
            return NULL;
        }
    }
    else if (!strcmp(t->type, LESSTHAN))
    {
        if (!strcmp(t->left->type, INTEGER) && !strcmp(t->right->type, INTEGER))
        {
            if (t->left->intVal < t->right->intVal)
            {
                result->intVal = 1;
            }
            else
            {
                result->intVal = 0;
            }
        }
        else
        {
            Fatal("need compare with integer.");
            printf("Type error: %s %s\n", t->left->type, t->right->type);
            exit(1);
            return NULL;
        }
    }
    else if (!strcmp(t->type, GREATEROREQUAL))
    {
        if (!strcmp(t->left->type, INTEGER) && !strcmp(t->right->type, INTEGER))
        {
            if (t->left->intVal >= t->right->intVal)
            {
                result->intVal = 1;
            }
            else
            {
                result->intVal = 0;
            }
        }
        else
        {
            Fatal("need compare with integer.");
            printf("Types error: %s %s\n", t->left->type, t->right->type);
            exit(1);
            return NULL;
        }
    }
    else if (!strcmp(t->type, LESSOREQUAL))
    {
        if (!strcmp(t->left->type, INTEGER) && !strcmp(t->right->type, INTEGER))
        {
            if (t->left->intVal <= t->right->intVal)
            {
                result->intVal = 1;
            }
            else
            {
                result->intVal = 0;
            }
        }
        else
        {
            Fatal("need compare with integer.");
            printf("Types error: %s %s\n", t->left->type, t->right->type);
            exit(1);
            return NULL;
        }
    }
    else if (!strcmp(t->type, ISEQUAL))
    {
        if (!strcmp(t->left->type, INTEGER) && !strcmp(t->right->type, INTEGER))
        {
            if (t->left->intVal == t->right->intVal)
            {
                result->intVal = 1;
            }
            else
            {
                result->intVal = 0;
            }
        }
        else if ((!strcmp(t->left->type, STRING) || !strcmp(t->left->type, IDENTIFIER)) && (!strcmp(t->right->type, STRING) || !strcmp(t->left->type, IDENTIFIER)))
        {
            result->intVal = !strcmp(t->left->stringVal, t->right->stringVal);
        }
        else if (!strcmp(t->left->type, NIL) && !strcmp(t->right->type, STRING))
        {
            result->intVal = 0;
        }
        else if (!strcmp(t->left->type, STRING) && !strcmp(t->right->type, NIL))
        {
            result->intVal = 0;
        }
        else if (!strcmp(t->left->type, NIL) && !strcmp(t->right->type, INTEGER))
        {
            result->intVal = 0;
        }
        else if (!strcmp(t->left->type, INTEGER) && !strcmp(t->right->type, NIL))
        {
            result->intVal = 0;
        }
        else if (!strcmp(t->left->type, STRING) && !strcmp(t->right->type, INTEGER))
        {
            result->intVal = 0;
        }
        else if (!strcmp(t->left->type, INTEGER) && !strcmp(t->right->type, STRING))
        {
            result->intVal = 0;
        }
        else if (!strcmp(t->left->type, NIL) && !strcmp(t->right->type, NIL))
        {
            result->intVal = 1;
        }
        else if (!strcmp(t->left->type, NIL) && strcmp(t->right->type, NIL))
        {
            result->intVal = 0;
        }
        else if (strcmp(t->left->type, NIL) && !strcmp(t->right->type, NIL))
        {
            result->intVal = 0;
        }
        else
        {
            printf("l->%s, r->%s\n", t->left->type, t->right->type);
            printf("compare error.");
            exit(1);
            return NULL;
        }
    }
    else if (!strcmp(t->type, AND))
    {
        if (isTrue(t->left) && isTrue(t->right))
        {
            result->intVal = 1;
        }
        else
        {
            result->intVal = 0;
        }
    }
    else if (!strcmp(t->type, OR))
    {
        if (isTrue(t->left) || isTrue(t->right))
        {
            result->intVal = 1;
        }
        else
        {
            result->intVal = 0;
        }
    }
    else if (!strcmp(t->type, PLUS))
    {
        return evalPlus(t, env);
    }
    else if (!strcmp(t->type, MINUS))
    {
        return evalMinus(t, env);
    }
    else if (!strcmp(t->type, DIVIDE))
    {
        return evalDivide(t, env);
    }
    else if (!strcmp(t->type, TIMES))
    {
        return evalTimes(t, env);
    }
    else if (!strcmp(t->type, EXPONENT))
    {
        return evalExponent(t, env);
    }
    else
    {
        return NULL;
    }
    return result;
}

Lexeme *evalArrayAccess(Lexeme *t, Lexeme *env)
{
    Lexeme *temp = lookupEnv(t->left, env);
    if (strcmp(temp->type, ARRAY))
    {
        Fatal("array error.\n");
        printf("Type: %s\n", temp->type);
        exit(1);
    }
    Lexeme *index = eval(t->right, env);
    if (strcmp(index->type, INTEGER))
    {
        Fatal("Must access array with INTEGER.\n");
        exit(1);
    }
    return temp->array[index->intVal];
}

// helpers
Lexeme *getFuncDefName(Lexeme *f)
{
    return car(f);
}

Lexeme *getFuncCallName(Lexeme *t)
{
    return car(t);
}

Lexeme *getFuncCallArgs(Lexeme *t)
{
    return car(cdr(t));
}

Lexeme *getClosureParams(Lexeme *t)
{
    return t->right->right->left->left;
}

Lexeme *getClosureBody(Lexeme *t)
{
    return cdr(cdr(cdr(t)));
}

Lexeme *getClosureEnvironment(Lexeme *t)
{
    return car(t);
}

int isTrue(Lexeme *t)
{
    if (t == NULL)
    {
        return 0;
    }
    else if (!strcmp(t->type, BAD))
    {
        return 0;
    }
    else if (!strcmp(t->type, INTEGER) && t->intVal == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

Lexeme *eval(Lexeme *tree, Lexeme *env)
{
    if (tree != NULL)
    {
        if (!strcmp(tree->type, STRING))
        {
            return tree;
        }
        else if (!strcmp(tree->type, INTEGER))
        {
            return tree;
        }
        else if (!strcmp(tree->type, ARRAY))
        {
            return tree;
        }
        else if (!strcmp(tree->type, CLOSURE))
        {
            return tree;
        }
        else if (!strcmp(tree->type, NIL))
        {
            return tree;
        }
        else if (!strcmp(tree->type, BREAK))
        {
            return tree;
        }
        else if (!strcmp(tree->type, DOT))
        {
            return evalDot(tree, env);
        }
        else if (!strcmp(tree->type, IDENTIFIER) || !strcmp(tree->type, FUNCTION))
        {
            if (!strcmp(tree->stringVal, "this"))
            {
                return env;
            }
            return lookupEnv(tree, env);
        }
        else if (!strcmp(tree->type, FUNCDEF))
        {
            return evalFuncDef(tree, env);
        }
        else if (!strcmp(tree->type, FUNCCALL))
        {
            return evalFuncCall(tree, env);
        }
        else if (!strcmp(tree->type, EXPRESSIONLIST))
        {
            return evalExpressionList(tree, env);
        }
        else if (!strcmp(tree->type, EXPR))
        {
            return evalExpr(tree, env);
        }
        else if (!strcmp(tree->type, LAMBDA))
        {
            return evalLambda(tree, env);
        }
        else if (!strcmp(tree->type, WHILE))
        {
            return evalWhile(tree, env);
        }
        else if (!strcmp(tree->type, IF))
        {
            return evalIf(tree, env);
        }
        else if (!strcmp(tree->type, BLOCK))
        {
            return evalBlock(tree, env);
        }
        else if (!strcmp(tree->type, OPTELSE))
        {
            return evalOptElse(tree, env);
        }
        else if (!strcmp(tree->type, FOR))
        {
            return evalFor(tree, env);
        }
        else if (!strcmp(tree->type, MINUS))
        {
            return evalMinus(tree, env);
        }
        else if (!strcmp(tree->type, PLUS))
        {
            return evalPlus(tree, env);
        }
        else if (!strcmp(tree->type, DIVIDE))
        {
            return evalDivide(tree, env);
        }
        else if (!strcmp(tree->type, TIMES))
        {
            return evalTimes(tree, env);
        }
        else if (!strcmp(tree->type, EXPONENT))
        {
            return evalExponent(tree, env);
        }
        else if (!strcmp(tree->type, NOT))
        {
            return evalSimpleOp(tree, env);
        }
        else if (!strcmp(tree->type, NOTEQUAL))
        {
            return evalSimpleOp(tree, env);
        }
        else if (!strcmp(tree->type, GREATERTHAN))
        {
            return evalSimpleOp(tree, env);
        }
        else if (!strcmp(tree->type, LESSTHAN))
        {
            return evalSimpleOp(tree, env);
        }
        else if (!strcmp(tree->type, GREATEROREQUAL))
        {
            return evalSimpleOp(tree, env);
        }
        else if (!strcmp(tree->type, LESSOREQUAL))
        {
            return evalSimpleOp(tree, env);
        }
        else if (!strcmp(tree->type, ISEQUAL))
        {
            return evalSimpleOp(tree, env);
        }
        else if (!strcmp(tree->type, AND))
        {
            return evalSimpleOp(tree, env);
        }
        else if (!strcmp(tree->type, OR))
        {
            return evalSimpleOp(tree, env);
        }
        else if (!strcmp(tree->type, EQUALS))
        {
            return evalAssign(tree, env);
        }
        else if (!strcmp(tree->type, ARRAYACCESS))
        {
            return evalArrayAccess(tree, env);
        }
        else
        {
            printf("Type: %s not evaluated.\n", tree->type);
        }
    }
    return NULL;
}