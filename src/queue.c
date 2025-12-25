#include "queue.h"

struct queue *createq() {
    struct queue *q = malloc(sizeof(struct queue));
    if (q == NULL) { perror("malloc"); exit(1); }
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

struct qnode *createqn(void *data) {
    struct qnode *qn = malloc(sizeof(struct qnode));
    if (qn == NULL) { perror("malloc"); exit(1); }

    qn->prev = qn->next = NULL;
    qn->data = data;

    return qn;
}

void freeq(struct queue *q) {
    struct qnode *qn = q->head;
    while (qn != NULL) {
        struct qnode *next = qn->next;
        free(qn);
        qn = next;
    }
    free(q);
}

void qpush(struct queue *q, void *data) {
    struct qnode *qn = createqn(data);
    qpushn(q, qn);
}

void qpushn(struct queue *q, struct qnode *qn) {
    if (q->head == NULL) {
        q->head = q->tail = qn;
    } else {
        q->tail->next = qn;
        qn->prev = q->tail;
        q->tail = qn;
    }
    q->size++;
}

struct qnode *qpop(struct queue *q) {
    if (q->head == NULL) { return NULL; }

    struct qnode *qn = q->head;
    q->head = q->head->next;

    if (q->head == NULL) { q->tail = NULL; }
    else { q->head->prev = NULL; }

    q->size--;
    return qn;
}

#ifdef DEBUG
void qprint(struct queue *q) {
    struct qnode *qn = q->head;
    while (qn != NULL) {
        qprintn(qn);
        qn = qn->next;
    }
}

void qprintn(struct qnode *qn) {
    printf("qnode: %p\n", qn);
    printf("  prev: %p\n", qn->prev);
    printf("  next: %p\n", qn->next);
    printf("  data: %p\n", qn->data);
}

#endif