#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//#define PARSEDEBUG
#ifdef PARSEDEBUG
#define DEBUG
#endif

#include "queue.h"
#include "util.h"
#include "lex.h"
#include "tree.h"

int tktype(struct queue *tok);
struct tnode *pConst(struct queue **tokens);
struct tnode *pUnary(struct queue **tokens);
struct tnode *pMultiplicative(struct queue **tokens);
struct tnode *pAdditive(struct queue **tokens);
struct tnode *pComparison(struct queue **tokens);
struct tnode *pEquality(struct queue **tokens);
struct tnode *pBitand(struct queue **tokens);
struct tnode *pBitxor(struct queue **tokens);
struct tnode *pBitor(struct queue **tokens);
struct tnode *pLogand(struct queue **tokens);
struct tnode *pLogor(struct queue **tokens);
struct tnode *pAssignment(struct queue **tokens);
struct tnode *idecltoken(struct queue **tokens, enum datatype e, int c, int r, int v, int s, int u);
struct tnode *pDeclaration(struct queue **tokens);
struct tnode *pBlock(struct queue **tokens);
struct tnode *pFunc (struct queue **tokens);
struct tnode *pStatement(struct queue **tokens);
struct tnode *pExpression(struct queue **tokens);
void tprint_tkn(struct tnode *tn, int depth);

#ifdef PARSEDEBUG
#endif