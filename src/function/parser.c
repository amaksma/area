#include "function/parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myutility.h"


Node from_string(const char *str) {
    assert(str);
    double value;
    if (sscanf(str, "%lf", &value) == 1) {
        return from_immediate(value);
    }
    
    for (OperatorType type = 0; type < NUM_OPERATORS; ++type) {
        if (strcmp(str, get_info(type)->name) == 0) {
            return from_operator(type);
        }
    }

    assert(str[1] == 0);
    return from_variable(*str);
}

Node *construct_tree(const char *src) {
    assert(src);
    char *str = malloc((strlen(src) + 1) * sizeof(*src));
    strcpy(str, src);

    size_t tokens = count_chars(str, ' ') + 1;
    Node **stack = malloc(tokens * sizeof(Node *));
    Node **sp = stack + tokens; // One past end
    
    char *token = strtok(str, " ");
    while (token) {
        Node *ptr = new(from_string(token));
        for (int i = get_argc(ptr); i-- > 0; ) {
            ptr->value.operator.args[i] = *(sp++); // pop
        }
        *(--sp) = ptr; // push

        token = strtok(NULL, " ");
    }

    Node *root = *sp;
    free(stack);
    free(str);
    
    return root;
}

void destruct_tree(Node *root) {
    for (int i = get_argc(root); i-- > 0; ) {
        destruct_tree(root->value.operator.args[i]);
    }
    free(root);
}
