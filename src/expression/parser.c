#include "expression/parser.h"
#include "expression/node.h"
#include "myutility.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Node *tree_from_string(const char *src) {
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
            ptr->value.operator.value.args[i] = *(sp++); // pop
        }
        *(--sp) = ptr; // push

        token = strtok(NULL, " ");
    }

    Node *root = *sp;
    free(stack);
    free(str);
    
    return root;
}


typedef struct {
    char name[8];
    Node node;
} PairNameNode;

Node from_string(const char *str) {
    PairNameNode names[] = {
        {"pi", from_named(NCONST_PI)},
        {"e", from_named(NCONST_E)},
        {"x", from_variable()},
        {"sin", from_unary_operator(UOP_SIN)},
        {"cos", from_unary_operator(UOP_COS)},
        {"tan", from_unary_operator(UOP_TAN)},
        {"ctg", from_unary_operator(UOP_CTG)},
        {"+", from_binary_operator(BOP_ADD)},
        {"-", from_binary_operator(BOP_SUB)},
        {"*", from_binary_operator(BOP_MUL)},
        {"/", from_binary_operator(BOP_DIV)},
    };

    double value;
    if (sscanf(str, "%lf", &value) == 1) {
        return from_immediate(value);
    }

    for (size_t i = 0; i < sizeof(names) / sizeof(*names); ++i) {
        if (strcmp(names[i].name, str) == 0) {
            return names[i].node;
        }
    }

    assert(0);
}


