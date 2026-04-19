#include "function/node.h"
#include <assert.h>
#include <stdlib.h>

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

Node *from_immediate(double value) {
    Node *ptr = new();
    *ptr = (Node) {
        .type = NODE_IMMEDIATE,
        .value.immediate = value
    };
    return ptr;
}

Node *from_variable(char variable) {
    Node *ptr = new();
    *ptr = (Node) {
        .type = NODE_VARIABLE,
        .value.variable = variable
    };
    return ptr;
}

Node *from_operator(OperatorType type) {
    Node *ptr = new();
    *ptr = (Node) {
        .type = NODE_OPERATOR,
        .value.operator.type = type
    };
    return ptr;
}