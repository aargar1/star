#include "lex.h"

struct token *itoken(enum tktype type, char *value, int line, int length) {
    struct token *t = malloc(sizeof(struct token));
    t->type = type;
    t->value = value;
    t->line = line;
    t->length = length;

    return t;
}

struct queue *lex(char *src) {
    struct queue *tokens = createq();

    int line = 0;
    int length = 0;

    for (int i = 0; i < strlen(src); i++) {        
        switch(src[i]) {
            case '\n':
                line++;
                break;

            case '{':
                qpush(tokens, itoken(TK_BRO, "{", line, length));
                break;
            
            case '}':
                qpush(tokens, itoken(TK_BRC, "}", line, length));
                break;
            
            case '(':
                qpush(tokens, itoken(TK_PRO, "(", line, length));
                break;
            
            case ')':
                qpush(tokens, itoken(TK_PRC, ")", line, length));
                break;
            
            case ';':
                qpush(tokens, itoken(TK_SEM, ";", line, length));
                break;

            case '-':
                if (src[i + 1] == '=') {
                    qpush(tokens, itoken(TK_MINEQ, "-=", line, length));
                    i++;
                } else if (src[i + 1] == '-') {
                    qpush(tokens, itoken(TK_MINUSMINUS, "--", line, length));
                    i++;
                } else {
                    qpush(tokens, itoken(TK_MIN, "-", line, length));
                }
                break;
            
            case '~':
                qpush(tokens, itoken(TK_BWC, "~", line, length));
                break;

            case '+':
                if (src[i + 1] == '=') {
                    qpush(tokens, itoken(TK_PLUSEQ, "+=", line, length));
                    i++;
                } else if (src[i + 1] == '+') {
                    qpush(tokens, itoken(TK_PLUSPLUS, "++", line, length));
                    i++;
                } else {
                    qpush(tokens, itoken(TK_ADD, "+", line, length));
                }
                break;
            
            case '*':
                if (src[i + 1] == '=') {
                    qpush(tokens, itoken(TK_MULEQ, "*=", line, length));
                    i++;
                } else {
                    qpush(tokens, itoken(TK_MUL, "*", line, length));
                }
                break;
            
            case '/':
                if (src[i + 1] == '=') {
                    qpush(tokens, itoken(TK_DIVEQ, "/=", line, length));
                    i++;
                } else {
                    qpush(tokens, itoken(TK_DIV, "/", line, length));
                }
                break;

            case '%':
                if (src[i + 1] == '=') {
                    qpush(tokens, itoken(TK_MODEQ, "%=", line, length));
                    i++;
                } else {
                    qpush(tokens, itoken(TK_MOD, "%", line, length));
                }
                break;

            case '=':
                if (src[i + 1] == '=') {
                    qpush(tokens, itoken(TK_EQEQ, "==", line, length));
                    i++;
                } else {
                    qpush(tokens, itoken(TK_EQ, "=", line, length));
                }
                break;
            case '<':
                if (src[i + 1] == '=') {
                    qpush(tokens, itoken(TK_LTEQ, "<=", line, length));
                    i++;
                } else if (src[i + 1] == '<') {
                    if (src[i + 2] == '=') {
                        qpush(tokens, itoken(TK_SHLEQ, "<<=", line, length));
                        i += 2;
                    } else {
                        qpush(tokens, itoken(TK_SHL, "<<", line, length));
                        i++;
                    }
                } else {
                    qpush(tokens, itoken(TK_LT, "<", line, length));
                }
                break;
            case '>':
                if (src[i + 1] == '=') {
                    qpush(tokens, itoken(TK_GTEQ, ">=", line, length));
                    i++;
                } else if (src[i + 1] == '>') {
                    if (src[i + 2] == '=') {
                        qpush(tokens, itoken(TK_SHREQ, ">>=", line, length));
                        i += 2;
                    } else {
                        qpush(tokens, itoken(TK_SHR, ">>", line, length));
                        i++;
                    }
                } else {
                    qpush(tokens, itoken(TK_GT, ">", line, length));
                }
                break;
            case '!':
                if (src[i + 1] == '=') {
                    qpush(tokens, itoken(TK_BANGEQ, "!=", line, length));
                    i++;
                } else {
                    qpush(tokens, itoken(TK_BANG, "!", line, length));
                }
                break;
            case '&':
                if (src[i + 1] == '&') {
                    qpush(tokens, itoken(TK_ANDAND, "&&", line, length));
                    i++;
                } else {
                    qpush(tokens, itoken(TK_AND, "&", line, length));
                }
                break;
            case '|':
                if (src[i + 1] == '|') {
                    qpush(tokens, itoken(TK_OROR, "||", line, length));
                    i++;
                } else {
                    qpush(tokens, itoken(TK_OR, "|", line, length));
                }
                break;
            case '^':
                qpush(tokens, itoken(TK_XOR, "^", line, length));
                break;
            default:
                if (isdigit(src[i])) {
                    int d = i;
                    while (isdigit(src[d])) {
                        d++;
                    }

                    struct token* t = malloc(sizeof(struct token));
                    t->type = TK_NUM;
                    t->line = line;
                    t->length = length;
                    t->value = malloc(d - i + 1);
                    strncpy(t->value, src + i, d - i);

                    qpush(tokens, t);

                    i = d - 1;

                } else if (isalpha(src[i])) {
                    int d = i;
                    while (!cspecial(src[d])) {
                        d++;
                    }

                    char *id = malloc(d - i + 1);
                    strncpy(id, src + i, d - i);

                    if (strcmp(id, "auto") == 0) {
                        qpush(tokens, itoken(TK_AUTO, id, line, length));
                    } else if (strcmp(id, "break") == 0) {
                        qpush(tokens, itoken(TK_BREAK, id, line, length));
                    } else if (strcmp(id, "case") == 0) {
                        qpush(tokens, itoken(TK_CASE, id, line, length));
                    } else if (strcmp(id, "char") == 0) {
                        qpush(tokens, itoken(TK_CHAR, id, line, length));
                    } else if (strcmp(id, "const") == 0) {
                        qpush(tokens, itoken(TK_CONST, id, line, length));
                    } else if (strcmp(id, "continue") == 0) {
                        qpush(tokens, itoken(TK_CONTINUE, id, line, length));
                    } else if (strcmp(id, "default") == 0) {
                        qpush(tokens, itoken(TK_DEFAULT, id, line, length));
                    } else if (strcmp(id, "do") == 0) {
                        qpush(tokens, itoken(TK_DO, id, line, length));
                    } else if (strcmp(id, "double") == 0) {
                        qpush(tokens, itoken(TK_DOUBLE, id, line, length));
                    } else if (strcmp(id, "else") == 0) {
                        qpush(tokens, itoken(TK_ELSE,id ,line ,length));
                    } else if (strcmp(id,"enum") == 0) {
                        qpush(tokens,itoken(TK_ENUM,id,line,length));
                    } else if (strcmp(id,"extern") == 0) {
                        qpush(tokens,itoken(TK_EXTERN,id,line,length));
                    } else if (strcmp(id,"float") == 0) {
                        qpush(tokens,itoken(TK_FLOAT,id,line,length));
                    } else if (strcmp(id,"for") == 0) {
                        qpush(tokens,itoken(TK_FOR,id,line,length));
                    } else if (strcmp(id,"goto") == 0) {
                        qpush(tokens,itoken(TK_GOTO,id,line,length));
                    } else if (strcmp(id,"if") == 0) {
                        qpush(tokens,itoken(TK_IF,id,line,length));
                    } else if (strcmp(id, "int") == 0) {
                        qpush(tokens, itoken(TK_INT, id, line, length));
                    } else if (strcmp(id,"inline") == 0) {
                        qpush(tokens,itoken(TK_INLINE,id,line,length));
                    } else if (strcmp(id,"long") == 0) {
                        qpush(tokens,itoken(TK_LONG,id,line,length));
                    } else if (strcmp(id,"register") == 0) {
                        qpush(tokens,itoken(TK_REGISTER,id,line,length));
                    } else if (strcmp(id,"restrict") == 0) {
                        qpush(tokens,itoken(TK_RESTRICT,id,line,length));
                    } else if (strcmp(id,"return") == 0) {
                        qpush(tokens,itoken(TK_RETURN,id,line,length));
                    } else if (strcmp(id,"short") == 0) {
                        qpush(tokens,itoken(TK_SHORT,id,line,length));
                    } else if (strcmp(id,"signed") == 0) {
                        qpush(tokens,itoken(TK_SIGNED,id,line,length));
                    } else if (strcmp(id,"sizeof") == 0) {
                        qpush(tokens,itoken(TK_SIZEOF,id,line,length));
                    } else if (strcmp(id,"static") == 0) {
                        qpush(tokens,itoken(TK_STATIC,id,line,length));
                    } else if (strcmp(id,"struct") == 0) {
                        qpush(tokens,itoken(TK_STRUCT,id,line,length));
                    } else if (strcmp(id,"switch") == 0) {
                        qpush(tokens,itoken(TK_SWITCH,id,line,length));
                    } else if (strcmp(id,"typedef") == 0) {
                        qpush(tokens,itoken(TK_TYPEDEF,id,line,length));
                    } else if (strcmp(id,"union") == 0) {
                        qpush(tokens,itoken(TK_UNION,id,line,length));
                    } else if (strcmp(id,"unsigned") == 0) {
                        qpush(tokens,itoken(TK_UNSIGNED,id,line,length));
                    } else if (strcmp(id,"void") == 0) {
                        qpush(tokens,itoken(TK_VOID,id,line,length));
                    } else if (strcmp(id,"volatile") == 0) {
                        qpush(tokens,itoken(TK_VOLATILE,id,line,length));
                    } else if (strcmp(id,"while") == 0) {
                        qpush(tokens,itoken(TK_WHILE,id,line,length));
                    } else {
                        qpush(tokens, itoken(TK_ID, id, line, length));
                    }

                    i = d - 1;
                }
        }

        length++;
    }

    qpush(tokens, itoken(TK_EOF, "EOF", line, length));
    return tokens;
}

#ifdef LEXDEBUG

int main (void) {
    char *src = "int main() { return 0; }";
    struct queue *tokens = lex(src);

    struct qnode *n = tokens->head;
    while (n != NULL) {
        struct token *t = n->data;
        printf("Token: %d %s\n", t->type, t->value);
        n = n->next;
    }
}

#endif