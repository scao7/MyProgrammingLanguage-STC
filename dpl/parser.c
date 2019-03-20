// #include "Fatal.h"
#include "parser.h"
#include "lexeme.h"
#include "lexer.h"
#include "type.h"

Lexeme *parse(FILE *inputFile)
{
    Parser *p = malloc(sizeof(Parser));

    p->fIn = inputFile;
    p->line = 1;
    p->pending = lex(p);
    p->tree = program(p);

    return p->tree;
}

int check(Parser *p, char *x)
{
    return strcmp(p->pending->type, x) == 0;
}

Lexeme *program(Parser *p)
{
    Lexeme *x = expressionList(p);
    check(p, ENDOFFILE);
    return x;
}

Lexeme *expressionList(Parser *p)
{
    Lexeme *x, *y = NULL;
    x = expression(p);
    if (expressionListPending(p))
    {
        y = expressionList(p);
    }
    return cons(EXPRESSIONLIST, x, y);
}

Lexeme *expression(Parser *p)
{
    if (loopPending(p))
    {
        return loop(p);
    }
    else if (ifStatementPending(p))
    {
        return ifStatement(p);
    }
    else if (funcDefPending(p))
    {
        return funcDef(p);
    }
    else if (exprPending(p))
    {
        Lexeme *x = expr(p);
        match(p, SEMICOLON);
        return x;
    }
    else
    {
        Fatal("Expression not found.");
        exit(1);
    }
    return NULL;
}

Lexeme *expr(Parser *p)
{
    Lexeme *x = NULL, *y = NULL, *z = NULL;
    x = primary(p);
    if (operatorPending(p))
    {
        y = operator(p);
        z = expr(p);
        return cons(EXPR, x, cons(GLUE, y, z));
    }
    //return cons(EXPR, x, cons(GLUE, y, z));
    return cons(EXPR, x, NULL);
}

Lexeme *optParamList(Parser *p)
{
    Lexeme *x = newLexeme(OPTPARAMLIST);
    if (paramListPending(p))
    {
        x->left = paramList(p);
    }
    return x;
}
Lexeme *paramList(Parser *p)
{
    Lexeme *x, *y = NULL;
    x = expr(p);
    if (check(p, COMMA))
    {
        match(p, COMMA);
        y = paramList(p);
    }
    return cons(PARAMLIST, x, y);
}

Lexeme *primary(Parser *p)
{
    Lexeme *x, *y = NULL;
    if (literalPending(p))
    {
        return literal(p);
    }
    else if (check(p, BREAK))
    {
        return match(p, BREAK);
    }
    else if (check(p, OPREN))
    {
        match(p, OPREN);
        x = expr(p);
        match(p, CPREN);
        return x;
    }
    else if (lambdaPending(p))
    {
        x = lambda(p);
        if (check(p, OPREN))
        {
            match(p, OPREN);
            y = optParamList(p);
            match(p, CPREN);
            return cons(FUNCCALL, x, y);
        }
        return x;
    }
    else if (check(p, NIL))
    {
        return match(p, NIL);
    }
    else if (check(p, IDENTIFIER))
    {
        x = match(p, IDENTIFIER);
        if (check(p, OBRACKET))
        {
            match(p, OBRACKET);
            y = expr(p);
            match(p, CBRACKET);
            return cons(ARRAYACCESS, x, y);
        }
        else if (check(p, OPREN))
        {
            match(p, OPREN);
            y = optParamList(p);
            match(p, CPREN);
            return cons(FUNCCALL, x, y);
        }
        else if (check(p, DOT))
        {
            y = match(p, DOT);
            y->left = x;
            y->right = primary(p);
            return y;
        }
        return x;
    }
    else
    {
        
        Fatal("Primary not found.");
        exit(1);
    }
    return NULL;
}

Lexeme *operator(Parser *p)
{
    if (check(p, MINUS))
    {
        return match(p, MINUS);
    }
    else if (check(p, PLUS))
    {
        return match(p, PLUS);
    }
    else if (check(p, DIVIDE))
    {
        return match(p, DIVIDE);
    }
    else if (check(p, TIMES))
    {
        return match(p, TIMES);
    }
    else if (check(p, EXPONENT))
    {
        return match(p, EXPONENT);
    }
    else if (check(p, NOT))
    {
        return match(p, NOT);
    }
    else if (check(p, NOTEQUAL))
    {
        return match(p, NOTEQUAL);
    }
    else if (check(p, GREATERTHAN))
    {
        return match(p, GREATERTHAN);
    }
    else if (check(p, LESSTHAN))
    {
        return match(p, LESSTHAN);
    }
    else if (check(p, GREATEROREQUAL))
    {
        return match(p, GREATEROREQUAL);
    }
    else if (check(p, LESSOREQUAL))
    {
        return match(p, LESSOREQUAL);
    }
    else if (check(p, ISEQUAL))
    {
        return match(p, ISEQUAL);
    }
    else if (check(p, AND))
    {
        return match(p, AND);
    }
    else if (check(p, OR))
    {
        return match(p, OR);
    }
    else if (check(p, EQUALS))
    {
        return match(p, EQUALS);
    }
    else if (check(p, DOT))
    {
        return match(p, DOT);
    }
    else
    {
        Fatal("Operator not found.");
        exit(1);
    }
    return NULL;
}

Lexeme *literal(Parser *p)
{
    if (check(p, INTEGER))
    {
        return match(p, INTEGER);
    }
    else if (check(p, STRING))
    {
        return match(p, STRING);
    }
    return NULL;
}

Lexeme *funcDef(Parser *p)
{
    Lexeme *name, *param, *body = NULL;
    match(p, FUNCTION);
    name = match(p, IDENTIFIER);
    match(p, OPREN);
    param = optIdList(p);
    match(p, CPREN);
    body = block(p);
    return cons(FUNCDEF, name, cons(GLUE, param, body));
}

Lexeme *optIdList(Parser *p)
{
    Lexeme *x = newLexeme(OPTIDLIST);
    if (idListPending(p))
    {
        x->left = idList(p);
    }
    return x;
}

Lexeme *idList(Parser *p)
{
    Lexeme *name, *list = NULL;
    name = match(p, IDENTIFIER);
    if (check(p, COMMA))
    {
        match(p, COMMA);
        list = idList(p);
    }
    return cons(IDLIST, name, list);
}

Lexeme *lambda(Parser *p)
{
    Lexeme *x, *y = NULL;
    match(p, LAMBDA);
    match(p, OPREN);
    x = optIdList(p);
    match(p, CPREN);
    y = block(p);
    return cons(LAMBDA, x, y);
}

Lexeme *loop(Parser *p)
{
    if (whileStatementPending(p))
    {
        return whileStatement(p);
    }
    else if (forStatementPending(p))
    {
        return forStatement(p);
    }
    else
    {
        Fatal("Loop not found.");
        exit(1);
    }
    return NULL;
}

Lexeme *block(Parser *p)
{
    match(p, OBRACE);
    Lexeme *l = cons(BLOCK, expressionList(p), NULL);
    match(p, CBRACE);
    return l;
}

Lexeme *whileStatement(Parser *p)
{
    Lexeme *x, *y = NULL;
    match(p, WHILE);
    match(p, OPREN);
    x = expr(p);
    match(p, CPREN);
    y = block(p);
    return cons(WHILE, x, y);
}

Lexeme *forStatement(Parser *p)
{
    Lexeme *x, *y, *z, *a = NULL;
    match(p, FOR);
    match(p, OPREN);
    x = expr(p);
    match(p, SEMICOLON);
    y = expr(p);
    match(p, SEMICOLON);
    z = expr(p);
    match(p, CPREN);
    a = block(p);
    return cons(FOR, cons(GLUE, x, y), cons(GLUE, z, a));
}

Lexeme *ifStatement(Parser *p)
{
    Lexeme *x, *y, *z = NULL;
    match(p, IF);
    match(p, OPREN);
    x = expr(p);
    match(p, CPREN);
    y = block(p);
    z = optElse(p);
    return cons(IF, x, cons(GLUE, y, z));
}

Lexeme *optElse(Parser *p)
{
    Lexeme *l = newLexeme(OPTELSE);
    if (check(p, ELSE))
    {
        match(p, ELSE);
        if (blockPending(p))
        {
            l->left = block(p);
            return l;
        }
        else if (ifStatementPending(p))
        {
            l->left = ifStatement(p);
            return l;
        }
        else
        {
            Fatal("optElse malformed.");
            exit(1);
        }
    }
    return NULL;
}

void pretty(Lexeme *l)
{
    prettyPrinter(l, "");
}

void prettyPrinter(Lexeme *l, char *s)
{
    if (l != NULL)
    {
        if (!strcmp(l->type, STRING))
        {
            printf("\"%s\"", l->stringVal);
        }
        else if (!strcmp(l->type, INTEGER))
        {
            printf("%d", l->intVal);
        }
        else if (!strcmp(l->type, IDENTIFIER) || !strcmp(l->type, FUNCTION))
        {
            printf("%s", l->stringVal);
        }
        else if (!strcmp(l->type, FUNCDEF))
        {
            printf("function ");
            prettyPrinter(l->left, "");
            printf("(");
            prettyPrinter(l->right->left, "");
            printf(")");
            prettyPrinter(l->right->right, "");
        }
        else if (!strcmp(l->type, FUNCCALL))
        {
            prettyPrinter(l->left, "");
            prettyPrinter(l->right, "");
        }
        else if (!strcmp(l->type, ARRAYACCESS))
        {
            prettyPrinter(l->left, "");
            printf("[");
            prettyPrinter(l->right, "");
            printf("]\n");
        }
        else if (!strcmp(l->type, EXPRESSIONLIST))
        {
            prettyPrinter(l->left, "");
            if (!strcmp(l->left->type, EXPR))
            {

                printf("; \n");
            }
            prettyPrinter(l->right, "");
        }
        else if (!strcmp(l->type, EXPR))
        {
            prettyPrinter(l->left, "");
            printf(" ");
            if (l->right != NULL)
            {
                prettyPrinter(l->right->left, "");
                prettyPrinter(l->right->right, "");
            }
        }
        else if (!strcmp(l->type, PARAMLIST))
        {
            prettyPrinter(l->left, "");
            if (l->right)
            {
                printf(", ");
            }
            prettyPrinter(l->right, "");
        }
        else if (!strcmp(l->type, PRIMARY))
        {
            prettyPrinter(l->left, "");
            prettyPrinter(l->right, "");
        }
        else if (!strcmp(l->type, LAMBDA))
        {
            prettyPrinter(l->left, "");
            prettyPrinter(l->right, "");
        }
        else if (!strcmp(l->type, WHILE))
        {
            printf("while (");
            prettyPrinter(l->left, "");
            printf(")\n");
            prettyPrinter(l->right, "");
        }
        else if (!strcmp(l->type, IF))
        {
            printf("if (");
            prettyPrinter(l->left, "");
            printf(")\n");
            prettyPrinter(l->right->left, "");
            prettyPrinter(l->right->right, "");
        }
        else if (!strcmp(l->type, BLOCK))
        {
            printf("{\n");
            prettyPrinter(l->left, "");
            printf("}\n");
        }
        else if (!strcmp(l->type, OPTPARAMLIST))
        {
            printf("(");
            prettyPrinter(l->left, "");
            printf(") ");
        }
        else if (!strcmp(l->type, OPTELSE))
        {
            if (l->left != NULL)
            {
                if (!strcmp(l->left->type, BLOCK))
                {
                    printf(" else ");
                    prettyPrinter(l->left, "");
                }
                else if (!strcmp(l->left->type, IF))
                {
                    printf(" else ");
                    prettyPrinter(l->left, "");
                }
            }
        }
        else if (!strcmp(l->type, FOR))
        {
            printf("for (");
            prettyPrinter(l->left->left, "");
            printf("; ");
            prettyPrinter(l->left->right, "");
            printf("; ");
            prettyPrinter(l->right->left, "");
            printf(") ");
            prettyPrinter(l->right->right, "");
        }
        else if (!strcmp(l->type, MINUS))
        {
            printf(" - ");
        }
        else if (!strcmp(l->type, PLUS))
        {
            printf(" + ");
        }
        else if (!strcmp(l->type, DIVIDE))
        {
            printf(" / ");
        }
        else if (!strcmp(l->type, TIMES))
        {
            printf(" * ");
        }
        else if (!strcmp(l->type, EXPONENT))
        {
            printf(" ^ ");
        }
        else if (!strcmp(l->type, NOT))
        {
            printf(" ! ");
        }
        else if (!strcmp(l->type, GREATERTHAN))
        {
            printf(" > ");
        }
        else if (!strcmp(l->type, LESSTHAN))
        {
            printf(" < ");
        }
        else if (!strcmp(l->type, GREATEROREQUAL))
        {
            printf(" >= ");
        }
        else if (!strcmp(l->type, LESSOREQUAL))
        {
            printf(" <= ");
        }
        else if (!strcmp(l->type, ISEQUAL))
        {
            printf(" == ");
        }
        else if (!strcmp(l->type, AND))
        {
            printf(" & ");
        }
        else if (!strcmp(l->type, OR))
        {
            printf(" | ");
        }
        else if (!strcmp(l->type, EQUALS))
        {
            printf(" = ");
        } 
    }
}
Lexeme *match(Parser *p, char *x)
{
    if (check(p, x))
    {
        return advance(p);
    }
    else
    {
        Fatal("FATAL: [can't match the type]");
        printf("-----need type: %s, but have: %s (line Line: %d)-----\n", p->pending->type, x, p->line);
        if (!strcmp(x, SEMICOLON))
        {
            printf("   Expected ';' after expression.\n");
        }
        else if (!strcmp(x, CPREN))
        {
            printf("   Expected ')' after IDENTIFIER or Parameter list.\n");
        }
        exit(1);
    }
}

Lexeme *advance(Parser *p)
{
    p->last = p->pending;
    p->pending = lex(p);
    return p->last;
}

// pending functions
int expressionListPending(Parser *p)
{
    return expressionPending(p);
}

int expressionPending(Parser *p)
{
    return exprPending(p) || loopPending(p) || ifStatementPending(p) || funcDefPending(p);
}

int exprPending(Parser *p)
{
    return primaryPending(p);
}

int optParamListPending(Parser *p)
{
    return paramListPending(p) || 1; 
}

int paramListPending(Parser *p)
{
    return exprPending(p);
}

int idListPending(Parser *p)
{
    return check(p, IDENTIFIER);
}

int primaryPending(Parser *p)
{
    return literalPending(p) || check(p, OPREN) || check(p, BREAK) ||
           lambdaPending(p) || variablePending(p) || check(p, NIL) || operatorPending(p);
}

int operatorPending(Parser *p)
{
    return check(p, MINUS) || check(p, PLUS) || check(p, DIVIDE) || check(p, TIMES) || check(p, EXPONENT) ||
           check(p, NOT) || check(p, GREATERTHAN) || check(p, LESSTHAN) || check(p, GREATEROREQUAL) || check(p, LESSOREQUAL) ||
           check(p, ISEQUAL) || check(p, NOTEQUAL) || check(p, AND) || check(p, OR) || check(p, EQUALS);
}

int literalPending(Parser *p)
{
    return check(p, INTEGER) || check(p, STRING);
}

int funcDefPending(Parser *p)
{
    return check(p, FUNCTION);
}

int lambdaPending(Parser *p)
{
    return check(p, LAMBDA);
}

int loopPending(Parser *p)
{
    return whileStatementPending(p) || forStatementPending(p);
}

int blockPending(Parser *p)
{
    return check(p, OBRACE);
}

int whileStatementPending(Parser *p)
{
    return check(p, WHILE);
}

int forStatementPending(Parser *p)
{
    return check(p, FOR);
}

int ifStatementPending(Parser *p)
{
    return check(p, IF);
}

int optElsePending(Parser *p)
{
    return check(p, ELSE) || 1; // ?? Do I need this?
}

int variablePending(Parser *p)
{
    return check(p, IDENTIFIER);
}



