/*
cs403 Shengting Cao
Programming Language
*/
#include <stdio.h>
#include <stdlib.h>
#include "lexeme.h"
#include "lexer.h"
#include "parser.h"
#include "environment.h"
#include "eval.h"
void error(char *e) {
    fprintf(stderr, "%s\n", e);
}

void setUpEnvironment(Lexeme *env);
// void setUpStdlib(Lexeme *global);
Lexeme *printSTC(Lexeme *args);
void printOutLexeme(Lexeme *x);
Lexeme *printlnSTC(Lexeme *args);
Lexeme *arraySTCBuildin(Lexeme *args);
Lexeme *arraySizeSTCBuildin(Lexeme *args);
//file operator not work, 
Lexeme *openFileForReading(Lexeme* args);
Lexeme *readInteger(Lexeme* args);
Lexeme *atFileEnd(Lexeme* args);
Lexeme* closeFile(Lexeme* args);

int main(int argc, char **argv, char **env)
{
    int pp = 0; 
    char *fname = argv[1];
    if (argc < 2)
    {
        fprintf(stderr, "can't find files \n");
        exit(1);
    }
    else if (argc == 3)
    {
        if (!strcmp(argv[1], "-pp"))
        {
            pp = 1;
            fname = argv[2];
        }
    }

    Lexeme *parseTree = parse(fopen(fname, "r"));
    if (pp)
    {
        pretty(parseTree);
        exit(0);
    }
    Lexeme *L = createEnv();
    setUpEnvironment(L);
    eval(parseTree, L);
    parseTree = NULL;
}

void setUpEnvironment(Lexeme *env)
{
    Lexeme *var = newLexeme(IDENTIFIER);
    var->stringVal = "print";
    Lexeme *val = newLexeme(BUILTIN);
    val->fval = printSTC;
    insert(var, val, env);

    var = newLexeme(IDENTIFIER);
    var->stringVal = "println";
    val = newLexeme(BUILTIN);
    val->fval = printlnSTC;
    insert(var, val, env);

    var = newLexeme(IDENTIFIER);
    var->stringVal = "array";
    val = newLexeme(BUILTIN);
    val->fval = arraySTCBuildin;
    insert(var, val, env);

    var = newLexeme(IDENTIFIER);
    var->stringVal = "arraySize";
    val = newLexeme(BUILTIN);
    val->fval = arraySizeSTCBuildin;
    insert(var, val, env);

    var = newLexeme(IDENTIFIER);
    var -> stringVal = "open";
    val = newLexeme(BUILTIN);
    val -> fval = openFileForReading;

    var = newLexeme(IDENTIFIER);
    var -> stringVal = "readInt";
    val = newLexeme(BUILTIN);
    val -> fval = openFileForReading;

    var = newLexeme(IDENTIFIER);
    var -> stringVal = "eof";
    val = newLexeme(BUILTIN);
    val -> fval = atFileEnd;

    var = newLexeme(IDENTIFIER);
    var -> stringVal = "close";
    val = newLexeme(BUILTIN);
    val -> fval = closeFile;

}

void printOutLexeme(Lexeme *x)
{
    if (!strcmp(x->type, INTEGER))
    {
        printf("%d", x->intVal);
    }
    else if (!strcmp(x->type, STRING))
    {
        printf("\"%s\"", x->stringVal);
    }
    else if (!strcmp(x->type, NIL))
    {
        printf("nil");
    }
    else
    {
        printf("can not print out this type:: %s\n", x->type);
        exit(1);
    }
}

Lexeme *printlnSTC(Lexeme *args)
{
    Lexeme *result = printSTC(args);
    printf("\n");
    return result;
}

Lexeme *arraySTCBuildin(Lexeme *args)
{
    if (args == NULL)
    {
        error("check your args, not enough for array\n");
        exit(1);
    }
    else if (args->right != NULL)
    {
        error("Too many arguments for array\n");
        exit(1);
    }
    else if (strcmp(args->left->type, INTEGER))
    {
        error("Argument to array must be an INTEGER\n");
        exit(1);
    }
    else if (args->left->intVal < 1)
    {
        error("Can't declare array with 0 or negative elements.\n");
        exit(1);
    }
    else
    {
        Lexeme *arr = newLexeme(ARRAY);
        arr->array = malloc(args->left->intVal * sizeof(Lexeme *));
        arr->intVal = args->left->intVal;
        for (int i = 0; i < args->left->intVal; i++)
        {
            arr->array[i] = newLexeme(NIL);
        }
        return arr;
    }
}

Lexeme *arraySizeSTCBuildin(Lexeme *args)
{
    if (args == NULL)
    {
        error("check your args, not enough for array\n");
        exit(1);
    }
    else if (args->right != NULL)
    {
        error("check your args, too much args for array\n");
        exit(1);
    }
    else if (strcmp(args->left->type, ARRAY))
    {
        error("Argument of array size must be an array\n");
        exit(1);
    }
    else
    {
        Lexeme *temp = newLexeme(INTEGER);
        temp->intVal = args->left->intVal;
        return temp;
    }
}

Lexeme *openFileForReading(Lexeme* args){
    Lexeme* fileName = args -> left;
    Lexeme* fp = newLexeme(FILEPOINTER);
    fp -> file = fopen(fileName ->stringVal, "r");;
    return fp;
};
Lexeme *readInteger(Lexeme* args){
    FILE* fp = args -> left -> file;
    int x;
    fscanf(fp,"%d",&x);
    Lexeme* newInt = newLexeme(INTEGER);
    newInt -> intVal = x;
    return newInt;
}
Lexeme *atFileEnd(Lexeme* args){
    FILE *fp = args -> left -> file;
    if(feof(fp)){
        Lexeme* newInt = newLexeme(INTEGER);
        newInt -> intVal = 1;
        return newInt;
    }
    else {
        Lexeme* newInt = newLexeme(INTEGER);
        newInt -> intVal = 0;
        return newInt;
    }
        
};
Lexeme* closeFile(Lexeme* args){    
    FILE* fp = args -> left -> file;
    fclose(fp);
    Lexeme*newInt =  newLexeme(INTEGER);
    newInt -> intVal = 1;
    return newInt;
};

Lexeme *printSTC(Lexeme *args)
{
    Lexeme *result = NULL;
    while (args != NULL)
    {
        if (!strcmp(args->left->type, INTEGER))
        {
            printf("%d", args->left->intVal);
        }
        else if (!strcmp(args->left->type, STRING))
        {
            printf("%s", args->left->stringVal);
        }
        else if (!strcmp(args->left->type, ARRAY))
        {
            int i;
            printf("[");
            for (i = 0; i < args->left->intVal; i++)
            {
                if (i == (args->left->intVal - 1))
                {
                    printOutLexeme(args->left->array[i]);
                }
                else
                {
                    printOutLexeme(args->left->array[i]);
                    printf(",");
                }
            }
            printf("]");
        }
        else if (!strcmp(args->left->type, CLOSURE))
        {
            pretty(args->left->right);
        }
        else
        {
            printf("can not print out this type: %s\n", args->left->type);
            exit(1);
        }
        result = args;
        args = args->right;
    }
    return result;
}