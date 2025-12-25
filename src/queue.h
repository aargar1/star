#pragma once

#include <stdlib.h>
#include <stdio.h>

struct qnode {
    struct qnode *prev;
    struct qnode *next;
    void *data;
};

struct queue {
    struct qnode *head;
    struct qnode *tail;
    size_t size;
};

struct queue *createq();
struct qnode *createqn(void *data);

void freeq(struct queue *q);

void qpush(struct queue *q, void *data);
void qpushn(struct queue *q, struct qnode *qn);
struct qnode *qpop(struct queue *q);

#ifdef DEBUG

void qprint(struct queue *q);
void qprintn(struct qnode *qn);

#endif