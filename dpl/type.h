/*
cs403 Shengting Cao
Programming Language
*/
#ifndef TYPE_H
#define TYPE_H
extern void Fatal(char* e);
// env types
extern char *ENVIRONMENT;
extern char *TABLE;
extern char *CLOSURE;
extern char *BUILTIN;

// large part
extern char *EXPRESSIONLIST;
extern char *GLUE;
extern char *EXPR;
extern char *EXPRESSION;
extern char *OPTPARAMLIST;
extern char *PARAMLIST;
extern char *PRIMARY;
extern char *OPERATOR;
extern char *LITERAL;
extern char *FUNCDEF;
extern char *FUNCCALL;
extern char *ARRAY;
extern char *ARRAYACCESS;
extern char *BLOCK;
extern char *OPTIDLIST;
extern char *IDLIST;

// small part
extern char *IDENTIFIER;
extern char *FUNCTION;
extern char *LAMBDA;
extern char *WHILE;
extern char *FOR;
extern char *IF;
extern char *OPTELSE;
extern char *ELSE;
extern char *BREAK;

extern char *DOT;
extern char *THIS;
extern char *MINUS;
extern char *PLUS;
extern char *DIVIDE;
extern char *TIMES;
extern char *EXPONENT;
extern char *NOT;
extern char *NOTEQUAL;
extern char *GREATERTHAN;
extern char *LESSTHAN;
extern char *GREATEROREQUAL;
extern char *LESSOREQUAL;
extern char *ISEQUAL;
extern char *AND;
extern char *OR;
extern char *EQUALS;

extern char *BAD;
extern char *NIL;

extern char *STRING;
extern char *INTEGER;

extern char *OPREN;
extern char *CPREN;
extern char *OBRACKET;
extern char *CBRACKET;
extern char *OBRACE;
extern char *CBRACE;
extern char *SEMICOLON;
extern char *COMMA;
extern char *ENDOFFILE;


extern char *FILEPOINTER;
#endif
