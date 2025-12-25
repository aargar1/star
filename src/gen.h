#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "tree.h"
#include "queue.h"
#include "parse.h"

int gen (struct tnode *tn, FILE *out);