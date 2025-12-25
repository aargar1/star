#include "tree.h"

struct tree *createt() {
    struct tree *t = malloc(sizeof(struct tree));
    if (t == NULL) { perror("malloc"); exit(1); }

    t->root = NULL;
    t->size = 0;

    return t;
}

struct tnode *createtn(void *data) {
    struct tnode *tn = malloc(sizeof(struct tnode));
    if (tn == NULL) { perror("malloc"); exit(1); }

    tn->parent = NULL;
    tn->children = createq();
    tn->data = data;

    return tn;
}

void freetn(struct tnode *tn) {
    if (tn != NULL && tn->children != NULL && tn->children->head != NULL) {
        struct qnode *qn = tn->children->head;
        while (qn != NULL) {
            struct qnode *next = qn->next;
            freetn(qn->data);
            free(qn);
            qn = next;
        }
    }
    if (tn != NULL && tn->children != NULL) free(tn->children);
    if (tn != NULL) free(tn);
}

void freet(struct tree *t) {
    freetn(t->root);
    free(t);
}

void tadd(struct tnode *parent, struct tnode *child) {
    qpush(parent->children, child);
    child->parent = parent;
}

struct tnode *taddt(struct tnode *parent, void *data) {
    struct tnode *tn = createtn(data);
    tadd(parent, tn);
    return tn;
}

struct tnode *trem(struct tnode *node) {
    struct tnode *parent = node->parent;
    struct qnode *qn = parent->children->head;
    while (qn != NULL) {
        if (qn->data == node) {
            qn->prev->next = qn->next;
            qn->next->prev = qn->prev;
            free(qn);
            break;
        }
        qn = qn->next;
    }
    return node;
}

#define DEBUG
#if defined(DEBUG) || defined(TREEDEBUG)

void tprintn(struct tnode *tn, int depth) {
    if (tn == NULL) return;

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    #if defined(TREEDEBUG)
    printf("- %s\n", (char *)tn->data);
    #elif defined(DEBUG)
    printf("- %p\n", (char *)tn->data);
    #endif

    struct qnode *qn = tn->children->head;
    while (qn != NULL) {
        tprintn(qn->data, depth + 1);
        qn = qn->next;
    }
}

void tprint(struct tree *t) {
    tprintn(t->root, 0);
}

#endif

#ifdef TREEDEBUG

int main (void) {
    struct tree *t = createt();
    char *data = "root";
    struct tnode *root = createtn(data);
    t->root = root;

    struct tnode *child1 = createtn("child1");
    tadd(root, child1);
    taddt(root, "child2");

    struct tnode *child3 = createtn("child3");
    tadd(child1, child3);

    tprint(t);

    freet(t);
}

#endif