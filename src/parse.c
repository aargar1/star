#include "parse.h"

int tktype (struct queue *tok) {
    if (tok == NULL || tok->head == NULL) return -1;
    struct token *t = (struct token *)tok->head->data;
    return t->type;
}

struct tnode *pConst(struct queue **tokens)  {
    if (tktype((*tokens)) == TK_NUM || tktype((*tokens)) == TK_ID) {
        struct token *t = (struct token *)(qpop(*tokens)->data);
        struct tnode *tn = createtn(t);
        return tn;
    }

    if (tktype((*tokens)) == TK_PRO) {
        qpop(*tokens);
        struct tnode *tn = pAssignment(tokens);

        if (tktype(*tokens) != TK_PRC) {
            perror("expected ')'\n");
            exit(1);
        }

        qpop(*tokens);
        return tn;
    }

    printf("tktype: %d\n", tktype(*tokens));
    perror("expected constant\n");
    exit(1);
}

struct tnode *pUnaryexp(struct queue **tokens) {
    if (tktype(*tokens) == TK_PLUSPLUS || tktype(*tokens) == TK_MINUSMINUS) {
        struct token *op = (struct token *)(qpop(*tokens)->data);
        op->type = (op->type == TK_PLUSPLUS) ? TK_PLUSPLUSPRE : TK_MINUSMINUSPRE;
        struct tnode *tn = pUnaryexp(tokens);
        struct tnode *optn = createtn(op);

        tadd(optn, tn);
        optn->parent = tn;
        return optn;
    }

    struct tnode *tn = pConst(tokens);

    if (tktype(*tokens) == TK_PLUSPLUS || tktype(*tokens) == TK_MINUSMINUS) {
        struct tnode *op = createtn((struct token *)(qpop(*tokens)->data));
        tadd(op, tn);

        op->parent = tn;
        return op;
    }

    return tn;
}

struct tnode *pUnary(struct queue **tokens) {
    if (tktype((*tokens)) == TK_BWC || tktype((*tokens)) == TK_BANG) {
        struct tnode *tn = createtn(qpop(*tokens)->data);
        tadd(tn, pUnary(tokens));
        return tn;

    } else if (tktype(*tokens) == TK_MIN) {
        int line = ((struct token *)(*tokens)->head->data)->line;
        int length = ((struct token *)(*tokens)->head->data)->length;

        struct token *t = itoken(TK_NEG, "-", line, length);
        struct tnode *tn = createtn(t);
        qpop(*tokens);

        tadd(tn, pUnary(tokens));
        return tn;
    } else if (tktype(*tokens) == TK_ADD) {
        qpop(*tokens);
    }

    return pUnaryexp(tokens);
}

struct tnode *pMultiplicative(struct queue **tokens) {
    struct tnode *tn = pUnary(tokens);
    while (tktype((*tokens)) == TK_MUL || tktype((*tokens)) == TK_DIV || tktype((*tokens)) == TK_MOD) {
        struct tnode *op = createtn(qpop(*tokens)->data);
        tadd(op, tn);

        op->parent = tn;
        tn = op;

        tadd(tn, pUnary(tokens));
    }

    return tn;
}

struct tnode *pAdditive(struct queue **tokens) {
    struct tnode *tn = pMultiplicative(tokens);
    while (tktype((*tokens)) == TK_ADD || tktype((*tokens)) == TK_MIN) {
        struct tnode *op = createtn(qpop(*tokens)->data);
        tadd(op, tn);

        op->parent = tn;
        tn = op;

        tadd(tn, pMultiplicative(tokens));
    }

    return tn;
}

struct tnode *pShift(struct queue **tokens) {
    struct tnode *tn = pAdditive(tokens);
    while (tktype((*tokens)) == TK_SHL || tktype((*tokens)) == TK_SHR) {
        struct tnode *op = createtn(qpop(*tokens)->data);
        tadd(op, tn);

        op->parent = tn;
        tn = op;

        tadd(tn, pAdditive(tokens));
    }

    return tn;
}

struct tnode *pComparison(struct queue **tokens) {
    struct tnode *tn = pShift(tokens);
    while (tktype((*tokens)) == TK_LT || tktype((*tokens)) == TK_GT || tktype((*tokens)) == TK_LTEQ || tktype((*tokens)) == TK_GTEQ) {
        struct tnode *op = createtn(qpop(*tokens)->data);
        tadd(op, tn);

        op->parent = tn;
        tn = op;

        tadd(tn, pShift(tokens));
    }

    return tn;
}

struct tnode *pEquality(struct queue **tokens) {
    struct tnode *tn = pComparison(tokens);
    while (tktype((*tokens)) == TK_EQEQ || tktype((*tokens)) == TK_BANGEQ) {
        struct tnode *op = createtn(qpop(*tokens)->data);
        tadd(op, tn);

        op->parent = tn;
        tn = op;

        tadd(tn, pComparison(tokens));
    }

    return tn;
}

struct tnode *pBitand(struct queue **tokens) {
    struct tnode *tn = pEquality(tokens);
    while (tktype((*tokens)) == TK_AND) {
        struct tnode *op = createtn(qpop(*tokens)->data);
        tadd(op, tn);

        op->parent = tn;
        tn = op;

        tadd(tn, pEquality(tokens));
    }
    return tn;
}

struct tnode *pBitxor(struct queue **tokens) {
    struct tnode *tn = pBitand(tokens);
    while (tktype((*tokens)) == TK_XOR) {
        struct tnode *op = createtn(qpop(*tokens)->data);
        tadd(op, tn);

        op->parent = tn;
        tn = op;

        tadd(tn, pBitand(tokens));
    }
    return tn;
}

struct tnode *pBitor(struct queue **tokens) {
    struct tnode *tn = pBitxor(tokens);
    while (tktype((*tokens)) == TK_OR) {
        struct tnode *op = createtn(qpop(*tokens)->data);
        tadd(op, tn);

        op->parent = tn;
        tn = op;

        tadd(tn, pBitxor(tokens));
    }
    return tn;
}

struct tnode *pLogand(struct queue **tokens) {
    struct tnode *tn = pBitor(tokens);
    while (tktype((*tokens)) == TK_ANDAND) {
        struct tnode *op = createtn(qpop(*tokens)->data);
        tadd(op, tn);

        op->parent = tn;
        tn = op;

        tadd(tn, pBitor(tokens));
    }
    return tn;
}

struct tnode *pLogor(struct queue **tokens) {
    struct tnode *tn = pLogand(tokens);
    while (tktype((*tokens)) == TK_OROR) {
        struct tnode *op = createtn(qpop(*tokens)->data);
        tadd(op, tn);

        op->parent = tn;
        tn = op;

        tadd(tn, pLogand(tokens));
    }
    return tn;
}

struct tnode* idecltoken(struct queue **tokens, enum datatype e, int c, int r, int v, int s, int u) {
    if (tktype((*tokens)) == TK_ID) {
        struct token *id = (struct token *)(qpop(*tokens)->data);
        struct tnode *tn = createtn(id);

        struct type *typeinfo = malloc(sizeof(struct type));
        typeinfo->type = e;
        typeinfo->const_mod = c;
        typeinfo->restrict_mod = r;
        typeinfo->volatile_mod = v;
        typeinfo->signed_mod = s;
        typeinfo->unsigned_mod = u;

        id->typeinfo = typeinfo;
        id->type = TK_DECL;

        tn->data = id;
        return tn;
    }

    else {
        perror("not a valid type combination\n");
        printf("%d\n", tktype(*tokens));
        exit(1);
    }
}

struct tnode* pDeclaration(struct queue **tokens) {

    int const_mod = 0;
    int restrict_mod = 0;
    int volatile_mod = 0;
    int signed_mod = 0;
    int unsigned_mod = 0;

    while (tktype((*tokens)) == TK_CONST || tktype((*tokens)) == TK_RESTRICT ||
           tktype((*tokens)) == TK_VOLATILE || tktype((*tokens)) == TK_SIGNED ||
           tktype((*tokens)) == TK_UNSIGNED) {

        if (tktype((*tokens)) == TK_CONST) {
            const_mod = 1;
        } else if (tktype((*tokens)) == TK_RESTRICT) {
            restrict_mod = 1;
        } else if (tktype((*tokens)) == TK_VOLATILE) {
            volatile_mod = 1;
        } else if (tktype((*tokens)) == TK_SIGNED) {
            signed_mod = 1;
        } else if (tktype((*tokens)) == TK_UNSIGNED) {
            unsigned_mod = 1;
        }

        qpop(*tokens);
    }

    if (tktype((*tokens)) == TK_LONG) {
        qpop(*tokens);
        if (tktype((*tokens)) == TK_INT) {
            qpop(*tokens);
            return (idecltoken(tokens, TYPE_LONG, const_mod, restrict_mod, volatile_mod, signed_mod, unsigned_mod));
        }

        else if (tktype((*tokens)) == TK_LONG) {
            qpop(*tokens);
            if (tktype((*tokens)) == TK_INT) qpop(*tokens);
            return (idecltoken(tokens, TYPE_LONG_LONG, const_mod, restrict_mod, volatile_mod, signed_mod, unsigned_mod));
        }

        else if (tktype((*tokens)) == TK_DOUBLE) {
            qpop(*tokens);
            return (idecltoken(tokens, TYPE_LONG_DOUBLE, const_mod, restrict_mod, volatile_mod, signed_mod, unsigned_mod));
        }

        return (idecltoken(tokens, TYPE_LONG, const_mod, restrict_mod, volatile_mod, signed_mod, unsigned_mod));
    }

    if (tktype((*tokens)) == TK_SHORT) {
        if (tktype((*tokens)) == TK_INT) qpop(*tokens);
        qpop(*tokens);
        return (idecltoken(tokens, TYPE_SHORT, const_mod, restrict_mod, volatile_mod, signed_mod, unsigned_mod));
    }

    if (tktype((*tokens)) == TK_CHAR || tktype((*tokens)) == TK_INT || tktype((*tokens)) == TK_FLOAT || tktype((*tokens)) == TK_DOUBLE) {
        qpop(*tokens);
        return (idecltoken(tokens, tktype(*tokens), const_mod, restrict_mod, volatile_mod, signed_mod, unsigned_mod));
    }

    return pLogor(tokens);
}

struct tnode *pAssignment(struct queue **tokens) {
    struct tnode *tn = pDeclaration(tokens);
    while (tktype((*tokens)) == TK_EQ || tktype((*tokens)) == TK_PLUSEQ || tktype((*tokens)) == TK_MINEQ || 
            tktype((*tokens)) == TK_MULEQ || tktype((*tokens)) == TK_DIVEQ || tktype((*tokens)) == TK_MODEQ ||
            tktype((*tokens)) == TK_SHLEQ || tktype((*tokens)) == TK_SHREQ) {

        struct tnode *op = createtn(qpop(*tokens)->data);
        tadd(op, tn);

        op->parent = tn;
        tn = op;

        tadd(tn, pDeclaration(tokens));
    }
    return tn;
}

struct tnode *pStatement (struct queue **tokens) {
    if (tktype((*tokens)) == TK_RETURN) {
        struct token *t = (struct token *)(qpop(*tokens)->data);
        struct tnode *tn = createtn(t);

        struct tnode *p = pAssignment(tokens);
        tadd(tn, p);

        if (tktype(*tokens) != TK_SEM) {
            perror("expected ';'\n");
            exit(1);
        }

        return tn;
    } else if (tktype((*tokens)) == TK_IF) {
        struct token *t = (struct token *)(qpop(*tokens)->data);
        qpop(*tokens);
        struct tnode *cond = pAssignment(tokens);

        if (tktype(*tokens) != TK_PRC) {
            perror("expected ')'\n");
            exit(1);
        }

        qpop(*tokens);

        struct tnode *block = pBlock(tokens);
        block->data = t;

        struct qnode *qn = createqn(cond);
        qn->next = block->children->head;
        block->children->head = qn;

        if (tktype((*tokens)) == TK_ELSE) {
            struct token *te = (struct token *)(qpop(*tokens)->data);

            struct tnode *blocke = pBlock(tokens);

            struct tnode *tne = createtn(te);

            if (strcmp(blocke->data, "block") == 0) {
                tne->children = blocke->children;
            } else {
                tadd(tne, blocke);
            }

            tadd(block, tne);
        }

        return block;

    } else if (tktype((*tokens)) == TK_WHILE) {
        struct token *t = (struct token *)(qpop(*tokens)->data);
        qpop(*tokens);
        struct tnode *cond = pAssignment(tokens);

        if (tktype(*tokens) != TK_PRC) {
            perror("expected ')'\n");
            exit(1);
        }

        qpop(*tokens);

        struct tnode *block = pBlock(tokens);
        block->data = t;

        struct qnode *qn = createqn(cond);
        qn->next = block->children->head;
        block->children->head = qn;

        return block;
    } else {
        struct tnode *tn = pAssignment(tokens);

        if (tktype(*tokens) != TK_SEM) {
            perror("expected ';'\n");
            printf("%d\n", tktype(*tokens));
            exit(1);
        }

        return tn;
    }

    return NULL;
}

struct tnode *pBlock (struct queue **tokens) {
    if (tktype(*tokens) == TK_BRO) {
        struct tnode *tn = createtn("block");
        qpop(*tokens);
        while (tktype(*tokens) != TK_BRC) {
            if (tktype(*tokens) == TK_SEM) {
                qpop(*tokens);
            } else {
                struct tnode *stmt = pStatement(tokens);
                tadd(tn, stmt);
            }

            if (tktype(*tokens) == TK_EOF) {
                perror("expected '}'\n");
                exit(1);
            }
        }

        qpop(*tokens);
        return tn;
    }

    return pStatement(tokens);
}

struct tnode *pFunc (struct queue **tokens) {
    if (tktype(*tokens) == TK_INT) {
        qpop(*tokens);
        if (tktype(*tokens) == TK_ID) {
            struct token *id = (struct token *)(qpop(*tokens)->data);
            if (tktype(*tokens) == TK_PRO) {
                while (tktype(*tokens) != TK_BRO) {
                    qpop(*tokens);
                }

                id->type = TK_FUN;
                
                struct tnode *tn = pBlock(tokens);
                tn->data = id;

                return tn;
            }
        }
    }

    return NULL;
}

void tprint_tkn(struct tnode *tn, int depth) {
    if (tn == NULL) return;

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    struct token *t = (struct token *)tn->data;
    printf("- %i %s\n", t->type, t->value);

    struct qnode *qn = tn->children->head;
    while (qn != NULL) {
        tprint_tkn(qn->data, depth + 1);
        qn = qn->next;
    }
}

#ifdef PARSEDEBUG
int main (void) {
    char *src = "int main() { if (1 == 1) {return 1;} else { return 2; } }";
    struct queue *tokens = lex(src);

    struct tnode *tn = pFunc(&tokens);
    tprint_tkn(tn, 0);
}
#endif