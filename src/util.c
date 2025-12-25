#include "util.h"

int isnumeric (char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }

    return 1;
}

int cspecial (char i) {
    if (!isalnum(i) && i != '_') {
        return 1;
    }

    return 0;
}