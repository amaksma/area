#include "function_parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myutility.h"




const OperatorInfo *get_info(OperatorType type) {
    return &op_info[type];
}

bool is_constant(const Node *self) {
    return self->type == NODE_IMMEDIATE;
}

bool is_variable(const Node *self) {
    return self->type == NODE_VARIABLE;
}

bool is_operator(const Node *self) {
    return self->type == NODE_OPERATOR;
}

int get_argc(const Node *self) {
    return is_operator(self) ? get_info(self->value.operator.type)->argc : 0;
}


Node *new(void) {
    return malloc(sizeof(Node));
}

Node from_immediate(double value) {
    return (Node) {
        .type = NODE_IMMEDIATE,
        .value.immediate = value
    };
}

Node from_variable(char variable) {
    return (Node) {
        .type = NODE_VARIABLE,
        .value.variable = variable
    };
}

Node from_operator(OperatorType type) {
    return (Node) {
        .type = NODE_OPERATOR,
        .value.operator.type = type
    };
}

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

Node *deep_copy(const Node *root) {
    assert(root);
    Node *copy = new();
    *copy = *root;
    if (is_operator(copy)) {
        for (int i = 0; i < get_argc(copy); ++i) {
            copy->value.operator.args[i] = deep_copy(copy->value.operator.args[i]);
        }
    }
    return copy;
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
        Node *ptr = new();
        *ptr = from_string(token);
        for (int i = get_argc(ptr); i-- > 0; ) {
            ptr->value.operator.args[i] = *(sp++);
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

