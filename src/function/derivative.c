#include "function/derivative.h"
#include "function/node.h"
#include "function/parser.h"
#include <assert.h>


static Node *derive_operator(const Node *root, char variable);

Node *derivative(const Node *root, char variable) {
    switch (root->type) {
    case NODE_IMMEDIATE:
        return from_immediate(0);
    case NODE_VARIABLE:
        return from_immediate(root->value.variable == variable ? 1 : 0);
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
