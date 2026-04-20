#include "function/simplify.h"
#include "function/node.h"
#include "function/parser.h"
#include "myutility.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

static double call_constant_operator(const Node *self) {
    assert(is_operator(self) && is_constant(self));
    switch (self->value.operator.type) {
    case OP_PI:
        return C_PI;
    case OP_E:
        return C_E;
    default:
        assert(0);
    }
}

static double const_value(const Node *self) {
    assert(is_constant(self));
    return is_operator(self) ? call_constant_operator(self) : self->value.immediate;
}

static Node *remove_left_if_equal(Node *root, Node *left, Node *right, double value) {
    if (is_immediate(left) && left->value.immediate == value) {
        destruct_tree(left);
        free(root);
        root = right;
    }
    return root;
}

static Node *remove_left_if_zero(Node *root, Node *left, Node *right) {
    return remove_left_if_equal(root, left, right, 0.0);
}

static Node *remove_left_if_one(Node *root, Node *left, Node *right) {
    return remove_left_if_equal(root, left, right, 1.0);
}

static Node *remove_right_if_left_zero(Node *root, Node *left, Node *right) {
    if (is_immediate(left) && left->value.immediate == 0.0) {
        destruct_tree(right);
        free(root);
        root = left;
    }
    return root;
}

static double addd(double left, double right) {
    return left + right;
}

static double subd(double left, double right) {
    return left - right;
}

static double muld(double left, double right) {
    return left * right;
}

static double divd(double left, double right) {
    return left / right;
}

static void calculate_if_const(Node *root, double (*f)(double, double)) {
    assert(is_operator(root) && get_argc(root) == 2);
    Node *left = root->value.operator.args[0];
    Node *right = root->value.operator.args[1];
    if (is_constant(left) && is_constant(right)) {
        double value = f(const_value(left), const_value(right));
        destruct_tree(left);
        destruct_tree(right);
        *root = from_immediate(value);
    }
}


Node *simplify(Node *root) {
    if (!is_operator(root)) return root;

    for (int i = 0; i < get_argc(root); ++i) {
        root->value.operator.args[i] = simplify(root->value.operator.args[i]);
    }

    bool unary_defined = false;
    double (*uf)(double);
    switch (root->value.operator.type) {
    case OP_PI :
    case OP_E  :
        break;
    case OP_SIN:
        if (!unary_defined) { unary_defined = true; uf = sin; }
    case OP_COS:
        if (!unary_defined) { unary_defined = true; uf = cos; }
    case OP_TAN:
        if (!unary_defined) { unary_defined = true; uf = tan; }
    case OP_CTG: {
        Node *arg = root->value.operator.args[0];
        if (!is_constant(arg)) break;

        double value = const_value(arg);
        destruct_tree(arg);
        *root = from_immediate(unary_defined ? uf(value) : 1 / tan(value));
        break;
    }
    case OP_ADD: {
        Node *left = root->value.operator.args[0];
        Node *right = root->value.operator.args[1];
        calculate_if_const(root, addd);
        root = remove_left_if_zero(root, left, right);
        root = remove_left_if_zero(root, right, left);
        break;
    }
    case OP_SUB: {
        Node *left = root->value.operator.args[0];
        Node *right = root->value.operator.args[1];
        calculate_if_const(root, subd);
        root = remove_left_if_zero(root, left, right);
        root = remove_left_if_zero(root, right, left);
        break;
    }
    case OP_MUL: {
        Node *left = root->value.operator.args[0];
        Node *right = root->value.operator.args[1];
        calculate_if_const(root, muld);
        root = remove_left_if_one(root, left, right);
        root = remove_left_if_one(root, right, left);
        root = remove_right_if_left_zero(root, left, right);
        root = remove_right_if_left_zero(root, right, left);
        break;
    }
    case OP_DIV: {
        Node *left = root->value.operator.args[0];
        Node *right = root->value.operator.args[1];
        calculate_if_const(root, divd);
        root = remove_left_if_one(root, left, right);
        root = remove_left_if_one(root, right, left);
        root = remove_right_if_left_zero(root, left, right);
        break;
    }
    }
    
    return root;
}
