#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "queue.h"
#include "parse.h"
#include "gen.h"

int gen(struct tnode *tn, FILE *out) {
    static int tmp = 1;
    static int lbl = 1;

    if (!tn || !tn->data) return 0;
    struct token *t = (struct token *)tn->data;

    switch (t->type) {
        case TK_FUN: {
            fprintf(out, "define i32 @%s() #0 {\n", t->value);
            while (tn->children && tn->children->head) {
                struct tnode *child = tn->children->head->data;
                gen(child, out);
                tn->children->head = tn->children->head->next;
            }
            fprintf(out, "  ret i32 0\n");
            fprintf(out, "}\n");
            return 0;
        }

        case TK_NUM: {
            int id = tmp++;
            fprintf(out, "  %%%i = add i32 0, %s\n", id, t->value);
            return id;
        }

        case TK_ID: {
            int id = tmp++;
            fprintf(out, "  %%%i = load i32, i32* %%%s, align 4\n", id, t->value);
            return id;
        }

        case TK_DECL: {
            fprintf(out, "  %%%s = alloca i32, align 4\n", t->value);
            return 0;
        }

        // ==================== BINARY OPERATORS ====================
        case TK_ADD: case TK_MIN: case TK_MUL: case TK_DIV: case TK_MOD:
        case TK_SHL: case TK_SHR: case TK_AND: case TK_OR: case TK_XOR:
        case TK_EQEQ: case TK_BANGEQ: case TK_LT: case TK_LTEQ:
        case TK_GT: case TK_GTEQ: case TK_ANDAND: case TK_OROR: {
            int lhs = gen(tn->children->head->data, out);
            int rhs = gen(tn->children->head->next->data, out);

            const char *op = "";
            int isCmp = 0;

            switch (t->type) {
                case TK_ADD: op="add"; break;
                case TK_MIN: op="sub"; break;
                case TK_MUL: op="mul"; break;
                case TK_DIV: op="sdiv"; break;
                case TK_MOD: op="srem"; break;
                case TK_SHL: op="shl"; break;
                case TK_SHR: op="lshr"; break;
                case TK_AND: op="and"; break;
                case TK_OR: op="or"; break;
                case TK_XOR: op="xor"; break;
                case TK_EQEQ: op="eq"; isCmp=1; break;
                case TK_BANGEQ: op="ne"; isCmp=1; break;
                case TK_LT: op="slt"; isCmp=1; break;
                case TK_LTEQ: op="sle"; isCmp=1; break;
                case TK_GT: op="sgt"; isCmp=1; break;
                case TK_GTEQ: op="sge"; isCmp=1; break;
                case TK_ANDAND: op="and"; break;
                case TK_OROR: op="or"; break;
            }

            int id = tmp++;
            if (isCmp) {
                fprintf(out, "  %%%i = icmp %s i32 %%%i, %%%i\n", id, op, lhs, rhs);
                fprintf(out, "  %%%i = zext i1 %%%i to i32\n", tmp, id);
                return tmp++;
            } else {
                fprintf(out, "  %%%i = %s i32 %%%i, %%%i\n", id, op, lhs, rhs);
                return id;
            }
        }

        // ==================== ASSIGNMENTS ====================
        case TK_EQ: case TK_PLUSEQ: case TK_MINEQ: case TK_MULEQ:
        case TK_DIVEQ: case TK_MODEQ: case TK_SHLEQ: case TK_SHREQ: {
            struct tnode *lhsNode = tn->children->head->data;
            struct tnode *rhsNode = tn->children->head->next->data;

            int lval = gen(lhsNode, out);

            if (t->type == TK_EQ) {
                fprintf(out, "  store i32 %%%i, i32* %%%s, align 4\n", gen(rhsNode, out), ((struct token *)(lhsNode->data))->value);
            } else {
                int rval = gen(rhsNode, out);
                const char *op="";
                switch(t->type){
                    case TK_PLUSEQ: op="add"; break;
                    case TK_MINEQ: op="sub"; break;
                    case TK_MULEQ: op="mul"; break;
                    case TK_DIVEQ: op="sdiv"; break;
                    case TK_MODEQ: op="srem"; break;
                    case TK_SHLEQ: op="shl"; break;
                    case TK_SHREQ: op="lshr"; break;
                }
                int id = tmp++;
                fprintf(out, "  %%%i = %s i32 %%%i, %%%i\n", id, op, lval, rval);
                fprintf(out, "  store i32 %%%i, i32* %%%s, align 4\n", id, ((struct token *)lhsNode->data)->value);
            }
            return 0;
        }

        case TK_PLUSPLUSPRE: case TK_MINUSMINUSPRE: case TK_PLUSPLUS: case TK_MINUSMINUS: {
            struct tnode *child = tn->children->head->data;
            int val = gen(child, out);
            fprintf(out, "  %%%i = add i32 %%%i, %s\n", tmp, val,
                    (t->type == TK_PLUSPLUSPRE || t->type == TK_PLUSPLUS) ? "1" : "-1");
            fprintf(out, "  store i32 %%%i, i32* %%%s, align 4\n", tmp,
                    ((struct token *)child->data)->value);
            return tmp++;
        }

        // ==================== UNARY ====================
        case TK_BWC: case TK_BANG: case TK_NEG: {
            int val = gen(tn->children->head->data, out);
            int id = tmp++;
            const char *op="";
            switch(t->type){
                case TK_BWC: op="not i1"; break;
                case TK_BANG: op="not i1"; break;
                case TK_NEG: op="sub i32 0,"; break;
            }
            fprintf(out, "  %%%i = %s %%%i\n", id, op, val);
            return id;
        }

        // ==================== RETURN ====================
        case TK_RETURN: {
            if (!tn->children || !tn->children->head)
                fprintf(out, "  ret i32 0\n");
            else {
                int val = gen(tn->children->head->data, out);
                fprintf(out, "  ret i32 %%%i\n", val);
            }
            return 0;
        }

        // ==================== BLOCK / STATEMENTS ====================
        case TK_BRO: {
            while (tn->children && tn->children->head) {
                struct tnode *child = tn->children->head->data;
                gen(child, out);
                tn->children->head = tn->children->head->next;
            }
            return 0;
        }

        // ==================== CONTROL FLOW ====================
        case TK_IF: {
            int cond = gen(tn->children->head->data, out);
            int trunc = tmp++;
            fprintf(out, "  %%%d = trunc i32 %%%d to i1\n", trunc, cond);

            int then_lbl = lbl++;
            int else_lbl = lbl++;
            int end_lbl = lbl++;

            fprintf(out, "  br i1 %%%d, label %%lbl%d, label %%lbl%d\n", trunc, then_lbl, else_lbl);
            fprintf(out, "lbl%d:\n", then_lbl);

            struct qnode *child = tn->children->head->next;
            while (child && ((struct token *)(((struct tnode *)(child->data))->data))->type != TK_ELSE) {
                gen(child->data, out);
                child = child->next;
            }

            fprintf(out, "  br label %%lbl%d\n", end_lbl);
            fprintf(out, "lbl%d:\n", else_lbl);
            
            if (child && ((struct token *)(((struct tnode *)(child->data))->data))->type == TK_ELSE) {
                struct tnode *else_node = ((struct tnode *)child->data);

                if (else_node && else_node->children) {
                    struct qnode *echild = else_node->children->head;
                    while (echild) {
                        gen(echild->data, out);
                        echild = echild->next;
                    }
                }

                fprintf(out, "  br label %%lbl%d\n", end_lbl);

            } else {
                fprintf(out, "  br label %%lbl%d\n", end_lbl);
            }

            fprintf(out, "lbl%d:\n", end_lbl);

            return 0;
        }

        case TK_WHILE: {
            int startlbl = lbl++;
            int condlbl = lbl++;
            int endlbl = lbl++;

            fprintf(out, "  br label %%lbl%i\n", condlbl);
            fprintf(out, "lbl%i:\n", condlbl);

            int cond = gen(tn->children->head->data, out);
            fprintf(out, "  %%%i = trunc i32 %%%i to i1\n", tmp++, cond);
            fprintf(out, "  br i1 %%%i, label %%lbl%i, label %%lbl%i\n", tmp-1, startlbl, endlbl);

            fprintf(out, "lbl%i:\n", startlbl);
            if (tn->children->head->next) gen(tn->children->head->next->data, out);
            fprintf(out, "  br label %%lbl%i\n", condlbl);

            fprintf(out, "lbl%i:\n", endlbl);
            return 0;
        }

        default:
            fprintf(stderr, "unsupported token type %d\n", t->type);
            return -1;
    }
}
