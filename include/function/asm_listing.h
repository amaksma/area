#ifndef FUNCTION_ASM_LISTING_H
#define FUNCTION_ASM_LISTING_H

#include "function/node.h"
#include <stdio.h>

void write_listing(
    FILE *stream,
    const Node *f1, const Node *f2, const Node *f3,
#ifdef NEWTON
    const Node *df1dx, const Node *df2dx, const Node *df3dx
#endif
);

#endif
