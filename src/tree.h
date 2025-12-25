#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "queue.h"

struct tnode {
    struct tnode *parent;
    struct queue *children;
    void *data;
};

struct tree {
    struct tnode *root;
    size_t size;
};

struct tree *createt();
struct tnode *createtn(void *data);

void freetn(struct tnode *tn);
void freet(struct tree *t);

void tadd(struct tnode *parent, struct tnode *child);
struct tnode *taddt(struct tnode *parent, void *data);
struct tnode *trem(struct tnode *node);

#ifdef DEBUG

void tprintn(struct tnode *tn, int depth);
void tprint(struct tree *t);

#endif