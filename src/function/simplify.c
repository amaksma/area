#include "function/simplify.h"
#include "function/node.h"
#include "function/parser.h"
#include <stdlib.h>

static bool is_equal(const Node *left, const Node *right) {
    if (left->type != right->type) { return false; }
    switch (left->type) {
    case NODE_IMMEDIATE:
        return left->value.immediate == right->value.immediate;
    case NODE_VARIABLE :
        return left->value.variable == right->value.variable;
    case NODE_OPERATOR :
        if (left->value.operator.type != right->value.operator.type) {
            return false;
        }
        for (int i = 0; i < get_argc(left); ++i) {
            if (!is_equal(left->value.operator.args[i], right->value.operator.args[i])) {
                return false;
            }
        }
    }
    return true;
}

Node *simplify(Node *root) {
    for (int i = 0; i < get_argc(root); ++i) {
        root->value.operator.args[i] = simplify(root->value.operator.args[i]);
    }
    switch (root->value.operator.type) {
    case OP_PI :
    case OP_E  :
    case OP_SIN:
    case OP_COS:
    case OP_TAN:
    case OP_CTG:
        break;
    case OP_ADD: {
        Node *left = root->value.operator.args[0];
        Node *right = root->value.operator.args[1];
        if (is_immediate(left) && is_immediate(right)) {
            double left_v = left->value.immediate;
            double right_v = right->value.immediate;
            destruct_tree(root);
            root = new(from_immediate(left_v + right_v));
            break;
        }
        if (is_immediate(left) && left->value.immediate == 0.0) {
            free(left);
            free(root);
            root = right;
            break;
        }
        if (is_immediate(right) && right->value.immediate == 0.0) {
            free(right);
            free(root);
            root = left;
            break;
        }
        break;
    }
    case OP_SUB: {
        Node *left = root->value.operator.args[0];
        Node *right = root->value.operator.args[1];
        if (is_immediate(left) && is_immediate(right)) {
            double left_v = left->value.immediate;
            double right_v = right->value.immediate;
            destruct_tree(root);
            root = new(from_immediate(left_v - right_v));
            break;
        }
        if (is_immediate(right) && right->value.immediate == 0.0) {
            free(right);
            free(root);
            root = left;
            break;
        }
        if (is_equal(left, right)) {
            destruct_tree(root);
            root = new(from_immediate(0));
            break;
        }
        break;
    }
    case OP_MUL: {
        Node *left = root->value.operator.args[0];
        Node *right = root->value.operator.args[1];
        if (is_immediate(left) && is_immediate(right)) {
            double left_v = left->value.immediate;
            double right_v = right->value.immediate;
            destruct_tree(root);
            root = new(from_immediate(left_v * right_v));
            break;
        }
        if (is_immediate(left) && left->value.immediate == 1.0) {
            free(left);
            free(root);
            root = right;
            break;
        }
        if (is_immediate(right) && right->value.immediate == 1.0) {
            free(right);
            free(root);
            root = left;
            break;
        }
        if ((is_immediate(left) && left->value.immediate == 0.0) || (is_immediate(right) && right->value.immediate == 0.0)) {
            destruct_tree(root);
            root = new(from_immediate(0));
            break;
        }
        break;
    }
    case OP_DIV: {
        Node *left = root->value.operator.args[0];
        Node *right = root->value.operator.args[1];
        if (is_immediate(left) && is_immediate(right)) {
            double left_v = left->value.immediate;
            double right_v = right->value.immediate;
            destruct_tree(root);
            root = new(from_immediate(left_v / right_v));
            break;
        }
        if (is_immediate(right) && right->value.immediate == 1.0) {
            free(right);
            free(root);
            root = left;
            break;
        }
        if ((is_immediate(left) && left->value.immediate == 0.0)) {
            destruct_tree(root);
            root = new(from_immediate(0));
            break;
        }
        if (is_equal(left, right)) {
            destruct_tree(root);
            root = new(from_immediate(1));
            break;
        }
        break;
    }
    }
    return root;
}
