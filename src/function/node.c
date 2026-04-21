#include "function/node.h"
#include <assert.h>
#include <stdlib.h>

const OperatorInfo *get_info(OperatorType type) {
    return &op_info[type];
}

bool is_constant(const Node *self) {
    return self->type == NODE_IMMEDIATE
        || (self->type == NODE_OPERATOR && get_argc(self) == 0);
}

bool is_immediate(const Node *self) {
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


Node *new(Node node) {
    Node *ptr = malloc(sizeof(Node));
    *ptr = node;
    return ptr;
}

Node *deep_copy(const Node *root) {
    assert(root);
    Node *copy = new(*root);
    // Recursively copy the child nodes
    for (int i = 0; i < get_argc(copy); ++i) {
        copy->value.operator.args[i] = deep_copy(copy->value.operator.args[i]);
    }
    return copy;
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

Node *substitute_variable(Node *root, const Node *src, char variable) {
    assert(root && src);
    for (int i = 0; i < get_argc(root); ++i) {
        root->value.operator.args[i] = substitute_variable(root->value.operator.args[i], src, variable);
    }
    // Replace the variable node with the copy of the provided tree
    if (is_variable(root) && root->value.variable == variable) {
        free(root);
        root = deep_copy(src);
    }
    return root;
}
