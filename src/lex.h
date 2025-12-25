#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "queue.h"
#include "util.h"

enum tktype {
    /* keywords */
    TK_AUTO,
    TK_BREAK,
    TK_CASE,
    TK_CHAR,
    TK_CONST,
    TK_CONTINUE,
    TK_DEFAULT,
    TK_DO,
    TK_DOUBLE,
    TK_ELSE,
    TK_ENUM,
    TK_EXTERN,
    TK_FLOAT,
    TK_FOR,
    TK_GOTO,
    TK_IF,
    TK_INLINE,
    TK_INT,
    TK_LONG,
    TK_REGISTER,
    TK_RESTRICT,
    TK_RETURN,
    TK_SHORT,
    TK_SIGNED,
    TK_SIZEOF,
    TK_STATIC,
    TK_STRUCT,
    TK_SWITCH,
    TK_TYPEDEF,
    TK_UNION,
    TK_UNSIGNED,
    TK_VOID,
    TK_VOLATILE,
    TK_WHILE,

    /* blocks */
    TK_FUN,
    
    /* literals */
    TK_NUM,
    TK_ID,
    TK_DECL,
    
    /* operators*/
    /* prepostfix */
    TK_PLUSPLUS,
    TK_MINUSMINUS,
    TK_PLUSPLUSPRE,
    TK_MINUSMINUSPRE,
    /* unary */
    TK_BWC,
    TK_BANG,
    TK_NEG,
    /* both */
    TK_ADD,
    /* binary */
    TK_MIN,
    TK_MUL,
    TK_DIV,
    TK_MOD,
    /* bitwise */
    TK_SHR,
    TK_SHL,
    TK_AND,
    TK_OR,
    TK_XOR,
    /* logical */
    TK_ANDAND,
    TK_OROR,
    /* assignment */
    TK_EQ,
    TK_PLUSEQ,
    TK_MINEQ,
    TK_MULEQ,
    TK_DIVEQ,
    TK_MODEQ,
    TK_SHREQ,
    TK_SHLEQ,
    /* comparisons */
    TK_EQEQ,
    TK_BANGEQ,
    TK_LT,
    TK_LTEQ,
    TK_GT,
    TK_GTEQ,
    /* flow */
    TK_BRO,
    TK_BRC,
    TK_PRO,
    TK_PRC,
    TK_SEM,

    /* file */
    TK_BOF,
    TK_EOF
};

enum datatype {
    TYPE_VOID,
    TYPE_CHAR,
    TYPE_SHORT,
    TYPE_INT,
    TYPE_LONG,
    TYPE_LONG_LONG,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_LONG_DOUBLE,
    TYPE_STRUCT,
    TYPE_UNION,
    TYPE_ENUM
};

struct type {
    enum datatype type;
    int const_mod;
    int restrict_mod;
    int volatile_mod;
    int signed_mod;
    int unsigned_mod;
};

struct token {
    enum tktype type;
    char *value;
    struct type *typeinfo;
    /* for errors */
    int line;
    int length;
};

struct token *itoken(enum tktype type, char *value, int line, int length);
struct queue *lex(char *src);