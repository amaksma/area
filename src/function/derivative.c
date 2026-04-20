#include "function/derivative.h"
#include "function/node.h"
#include "function/parser.h"
#include <assert.h>


static Node *derive_operator(const Node *root, char variable);

Node *derivative(const Node *root, char variable) {
    switch (root->type) {
    case NODE_IMMEDIATE:
        return new(from_immediate(0));
    case NODE_VARIABLE:
        return new(from_immediate(root->value.variable == variable ? 1 : 0));
    case NODE_OPERATOR:
        return derive_operator(root, variable);
    }
}

Node *derive_operator(const Node *root, char variable) {
    assert(is_operator(root) && get_argc(root) < 3);
    Node *ptr = construct_tree(get_info(root->value.operator.type)->derivative);
    switch (get_argc(root)) {
    case 0:
        break;
    case 1: {
        Node *F = deep_copy(root->value.operator.args[0]);
        Node *f = derivative(F, variable);
        
        substitute_variable(ptr, F, 'F');
        substitute_variable(ptr, f, 'f');

        destruct_tree(f);
        destruct_tree(F);
        break;
    }
    case 2: {
        Node *F = deep_copy(root->value.operator.args[0]);
        Node *f = derivative(F, variable);
        Node *G = deep_copy(root->value.operator.args[1]);
        Node *g = derivative(G, variable);

        substitute_variable(ptr, F, 'F');
        substitute_variable(ptr, f, 'f');
        substitute_variable(ptr, G, 'G');
        substitute_variable(ptr, g, 'g');

        destruct_tree(g);
        destruct_tree(G);
        destruct_tree(f);
        destruct_tree(F);
        break;
    }
    }
    return ptr;
}

static Node from_operator_unary(OperatorType type, Node *arg) {
    Node node = from_operator(type);
    node.value.operator.args[0] = arg;
    return node;
}
static Node from_operator_binary(OperatorType type, Node *left, Node *right) {
    Node node = from_operator(type);
    node.value.operator.args[0] = left;
    node.value.operator.args[1] = right;
    return node;
}

Node *derive_operator1(const Node *root, char variable) {
    assert(is_operator(root) && get_argc(root) < 3);
    Node *ptr = deep_copy(root);
    switch (ptr->value.operator.type) {
    case OP_PI :
    case OP_E  :
        *ptr = from_immediate(0);
        break;
    case OP_SIN: {
        Node *arg = ptr->value.operator.args[0];
        if (is_constant(arg) || (is_variable(arg) && arg->value.variable != variable)) {
            *ptr = from_immediate(0);
        } else if (is_variable(arg)) {
            *ptr = from_operator_unary(OP_COS, arg);
        } else {
            *ptr = from_operator_binary(OP_MUL, derivative(arg, variable), new(from_operator_unary(OP_COS, arg)));
        }
    }
        
    case OP_COS:
    case OP_TAN:
    case OP_CTG:
    case OP_ADD:
    case OP_SUB:
    case OP_MUL:
    case OP_DIV: break;
    }
    return ptr;
}

