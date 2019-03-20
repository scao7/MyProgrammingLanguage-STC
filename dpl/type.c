/*
cs403 Shengting Cao
Programming Language
*/
#include "type.h"
#include <stdio.h>

void Fatal(char *e) {
    fprintf(stderr, "%s\n", e);
}

// env types
char *ENVIRONMENT = "ENVIRONMENT";
char *TABLE = "TABLE";
char *CLOSURE = "CLOSURE";
char *BUILTIN = "BUILTIN";

// higher level types
char *EXPRESSIONLIST = "EXPRESSIONLIST";
char *GLUE = "GLUE";
char *EXPR = "EXPR";
char *EXPRESSION = "EXPRESSION";
char *OPTPARAMLIST = "OPTPARAMLIST";
char *PARAMLIST = "PARAMLIST";
char *PRIMARY = "PRIMARY";
char *OPERATOR = "OPERATOR";
char *LITERAL = "LITERAL";
char *FUNCDEF = "FUNCDEF";
char *FUNCCALL = "FUNCCALL";
char *ARRAY = "ARRAY";
char *ARRAYACCESS = "ARRAYACCESS";
char *BLOCK = "BLOCK";
char *OPTIDLIST = "OPTIDLIST";
char *IDLIST = "IDLIST";

// lower level types
char *IDENTIFIER = "IDENTIFIER";
char *FUNCTION = "FUNCTION";
char *LAMBDA = "LAMBDA";
char *WHILE = "WHILE";
char *FOR = "FOR";
char *IF = "IF";
char *ELSE = "ELSE";
char *OPTELSE = "OPTELSE";
char *BREAK = "BREAK";

char *MINUS = "MINUS";
char *PLUS = "PLUS";
char *DIVIDE = "DIVIDE";
char *TIMES = "TIMES";
char *EXPONENT = "EXPONENT";
char *NOT = "NOT";
char *NOTEQUAL = "NOTEQUAL";
char *GREATERTHAN = "GREATERTHAN";
char *LESSTHAN = "LESSTHAN";
char *GREATEROREQUAL = "GREATEROREQUAL";
char *LESSOREQUAL = "LESSOREQUAL";
char *ISEQUAL = "ISEQUAL";
char *AND = "AND";
char *OR = "OR";
char *EQUALS = "EQUALS";

char *BAD = "BAD";
char *NIL = "NIL";

char *STRING = "STRING";
char *INTEGER = "INTEGER";

char *DOT = "DOT";
char *THIS = "THIS";
char *OPREN = "OPREN";
char *CPREN = "CPREN";
char *OBRACKET = "OBRACKET";
char *CBRACKET = "CBRACKET";
char *OBRACE = "OBRACE";
char *CBRACE = "CBRACE";
char *SEMICOLON = "SEMICOLON";
char *COMMA = "COMMA";
char *ENDOFFILE = "ENDOFFILE";

char *FILEPOINTER = "FILEPOINTER";