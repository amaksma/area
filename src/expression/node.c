#include "expression/node.h"
#include <stdlib.h>


bool is_constant(const Node *self) {
    return self->type == NODE_CONSTANT;
}

bool is_variable(const Node *self) {
    return self->type == NODE_VARIABLE;
}

bool is_operator(const Node *self) {
    return self->type == NODE_OPERATOR;
}

bool is_imm_constant(const Node *self) {
    return is_constant(self) && self->value.constant.type == CONST_IMMEDIATE;
}

bool is_named_constant(const Node *self) {
    return is_constant(self) && self->value.constant.type == CONST_NAMED;
}

bool is_unary_operator(const Node *self) {
    return is_operator(self) && self->value.operator.type == OP_UNARY;
}

bool is_binary_operator(const Node *self) {
    return is_operator(self) && self->value.operator.type == OP_BINARY;
}

int get_argc(const Node *self) {
    if (is_unary_operator(self))  { return 1; }
    if (is_binary_operator(self)) { return 2; }
    return 0;
}



Node *new(Node node) {
    Node *ptr = malloc(sizeof(Node));
    *ptr = node;
    return ptr;
}

Node *deep_copy(const Node *root) {
    if (!root) { return NULL; }
    Node *ptr = new(*root);
    for (int i = 0; i < get_argc(ptr); ++i) {
        Node **arg = &ptr->value.operator.value.args[i];
        *arg = deep_copy(*arg);
    }
    return ptr;
}

void deep_free(Node *root) {
    for (int i = 0; i < get_argc(root); ++i) {
        deep_free(root->value.operator.value.args[i]);
    }
    free(root);
}



Node from_immediate(double value) {
    return (Node) {
        .type = NODE_CONSTANT,
        .value.constant.type = CONST_IMMEDIATE,
        .value.constant.value.immediate = value
    };
}

Node from_named(NamedConstant name) {
    return (Node) {
        .type = NODE_CONSTANT,
        .value.constant.type = CONST_NAMED,
        .value.constant.value.name = name
    };
}

Node from_variable(void) {
    return (Node) {
        .type = NODE_VARIABLE
    };
}

Node from_unary_operator(UnaryOperator type) {
    return from_unary_operator_args(type, NULL);
}

Node from_binary_operator(BinaryOperator type) {
    return from_binary_operator_args(type, NULL, NULL);
}

Node from_unary_operator_args(UnaryOperator type, Node *arg) {
    return (Node) {
        .type = NODE_OPERATOR,
        .value.operator.type = OP_UNARY,
        .value.operator.value.unary.type = type,
        .value.operator.value.unary.arg = arg
    };
}

Node from_binary_operator_args(BinaryOperator type, Node *left, Node *right) {
    return (Node) {
        .type = NODE_OPERATOR,
        .value.operator.type = OP_BINARY,
        .value.operator.value.binary.type = type,
        .value.operator.value.binary.left = left,
        .value.operator.value.binary.right = right
    };
}
