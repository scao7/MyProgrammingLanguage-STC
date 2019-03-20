#include "lexeme.h"
#include "lexer.h"
#include "parser.h"
#include "type.h"

Lexeme *lex(Parser *p)
{
    int ch;
    skipWhiteSpace(p);
    ch = getChar(p);
    if (ch == EOF)
        return newLexeme(ENDOFFILE);

    switch (ch)
    {
    // single character tokens
    case '.':
        return newLexeme(DOT);
    case '-':
        return newLexeme(MINUS);
    case '+':
        return newLexeme(PLUS);
    case '/':
        return newLexeme(DIVIDE);
    case '*':
        return newLexeme(TIMES);
    case '^':
        return newLexeme(EXPONENT);
    case '!':
        ch = getChar(p);
        if (ch == '=')
        {
            return newLexeme(NOTEQUAL);
        }
        ungetc(ch, p->fIn);
        return newLexeme(NOT);
    case '>':
        ch = getChar(p);
        if (ch == '=')
        {
            return newLexeme(GREATEROREQUAL);
        }
        ungetc(ch, p->fIn);
        return newLexeme(GREATERTHAN);
    case '<':
        ch = getChar(p);
        if (ch == '=')
        {
            return newLexeme(LESSOREQUAL);
        }
        ungetc(ch, p->fIn);
        return newLexeme(LESSTHAN);
    case '&':
        return newLexeme(AND);
    case '|':
        return newLexeme(OR);
    case '=':
        ch = getChar(p);
        if (ch == '=')
        {
            return newLexeme(ISEQUAL);
        }
        ungetc(ch, p->fIn);
        return newLexeme(EQUALS);
    case '(':
        return newLexeme(OPREN);
    case ')':
        return newLexeme(CPREN);
    case '[':
        return newLexeme(OBRACKET);
    case ']':
        return newLexeme(CBRACKET);
    case '{':
        return newLexeme(OBRACE);
    case '}':
        return newLexeme(CBRACE);
    case ';':
        return newLexeme(SEMICOLON);
    case ',':
        return newLexeme(COMMA);

    default:
        if (isdigit(ch))
        {
            return lexNumber(p, ch);
        }
        else if (isalpha(ch))
        {
            return lexID(p, ch); 
        }
        else if (ch == '\"')
        {
            return lexString(p, ch);
        }
    }
    return newLexeme(BAD);
}

Lexeme *lexNumber(Parser *p, int i)
{
    Lexeme *l = newLexeme(INTEGER);
    char n[64] = "";
    int size = 1;
    n[0] = i;
    i = getChar(p);

    while (!isWhiteSpace(i) && isdigit(i))
    {
        n[size++] = i;
        i = getChar(p);
    }

    ungetc(i, p->fIn);
    n[size++] = '\0';
    l->intVal = atoi(n);
    l->stringVal = n;

    return l;
}

Lexeme *lexID(Parser *p, int i)
{
    Lexeme *l = newLexeme(IDENTIFIER);
    char s[32] = "";
    int size = 1;
    s[0] = i;
    i = getChar(p);

    while (!isWhiteSpace(i) && (isalpha(i) || isdigit(i)))
    {
        s[size++] = i;
        i = getChar(p);
    }

    ungetc(i, p->fIn);
    s[size++] = '\0';

    if (!strcmp(s, "function"))
    {
        l->type = FUNCTION;
    }
    else if (!strcmp(s, "lambda"))
    {
        l->type = LAMBDA;
    }
    else if (!strcmp(s, "while"))
    {
        l->type = WHILE;
    }
    else if (!strcmp(s, "for"))
    {
        l->type = FOR;
    }
    else if (!strcmp(s, "if"))
    {
        l->type = IF;
    }
    else if (!strcmp(s, "else"))
    {
        l->type = ELSE;
    }
    else if (!strcmp(s, "nil"))
    {
        l->type = NIL;
    }
    else if (!strcmp(s, "break"))
    {
        l->type = BREAK;
    }
    else
    {
        l->stringVal = malloc(strlen(s) + 1);
        strcpy(l->stringVal, s);
    }
    return l;
}

Lexeme *lexString(Parser *p, int i)
{
    Lexeme *l = newLexeme(STRING);
    char s[128] = "";
    int size = 0;
    i = getChar(p);

    while (i != '\"')
    {
        s[size] = i;
        size++;
        i = getChar(p);
    }

    s[size++] = '\0';
    l->stringVal = malloc(strlen(s) + 1);
    strcpy(l->stringVal, s);
    l->intVal = size;
    return l;
}

int checkNewLine(int cur)
{
    return cur == '\r' || cur == '\n' || cur == '\t';
}

int isWhiteSpace(int cur)
{
    return checkNewLine(cur) || cur == ' ';
}

int getChar(Parser *p)
{
    int cur = fgetc(p->fIn);
    if (cur == '/')
    {
        int curr = fgetc(p->fIn);
        if (curr == '/')
        {
            while (!checkNewLine(cur) && !feof(p->fIn))
            {
                cur = fgetc(p->fIn);
            }
        }
        else
        {
            ungetc(curr, p->fIn);
        }
    }
    return cur;
}

void skipWhiteSpace(Parser *p)
{
    int cur = getChar(p);
    while (isWhiteSpace(cur) && !feof(p->fIn))
    {
        if (checkNewLine(cur))
        {
            p->line++;
        }
        cur = getChar(p);
    }
    ungetc(cur, p->fIn);
}
