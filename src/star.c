#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include "tree.h"
#include "queue.h"
#include "parse.h"
#include "lex.h"
#include "gen.h"

int main (int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <in> <out>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "rb");
    if (!in) {
        printf("Error (fopen): %s\n", strerror(errno));
        return 1;
    }

    if (fseek(in, 0, SEEK_END) != 0) {
        printf("Error (fseek): %s\n", strerror(errno));
        fclose(in);
        return 1;
    }

    long size = ftell(in);
    if (size < 0) {
        printf("Error (ftell): %s\n", strerror(errno));
        fclose(in);
        return 1;
    }
    rewind(in);

    char *src = malloc((size_t)size + 1);
    if (!src) {
        printf("Error (malloc): %s\n", strerror(errno));
        fclose(in);
        return 1;
    }

    fread(src, 1, (size_t)size, in);
    src[size] = '\0';
    fclose(in);

    struct queue *tokens = lex(src);
    struct tnode *tree = pFunc(&tokens);

    FILE *out = fopen(argv[2], "w");
    if(!out) {
        printf("Error (fopen): %s\n", strerror(errno));
        return 1;
    }

    gen(tree, out);

    freetn(tree);
    freeq(tokens);
    fclose(out);

    return 0;
}