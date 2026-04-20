#include "expression/derivative.h"
#include "expression/node.h"



static Node derive_unary_operator(const Node *root) {
    const Node *arg = root->value.operator.value.unary.arg;
    if (is_constant(arg)) {
        return from_immediate(0);
    }

    switch (root->value.operator.value.unary.type) {
    case UOP_SIN:
        return from_binary_operator_args(BOP_MUL,
            new(from_unary_operator_args(UOP_COS, deep_copy(arg))),
            derivative(arg)
        );
    case UOP_COS:
        return from_binary_operator_args(BOP_MUL,
            new(from_unary_operator_args(UOP_SIN, deep_copy(arg))),
            new(from_binary_operator_args(BOP_MUL, new(from_immediate(-1)), derivative(arg)))
        );
    case UOP_TAN: {
        Node *cos_arg = new(from_unary_operator_args(UOP_COS, deep_copy(arg)));
        Node *cos_sq = new(from_binary_operator_args(BOP_MUL, cos_arg, deep_copy(cos_arg)));
        return from_binary_operator_args(BOP_DIV, derivative(arg), cos_sq);
    }
    case UOP_CTG: {
        Node *sin_arg = new(from_unary_operator_args(UOP_SIN, deep_copy(arg)));
        Node *sin_sq = new(from_binary_operator_args(BOP_MUL, sin_arg, deep_copy(sin_arg)));
        Node *neg_d = new(from_binary_operator_args(BOP_MUL, derivative(arg), new(from_immediate(-1))));
        return from_binary_operator_args(BOP_DIV, neg_d, sin_sq);
    }
    }
}


Node *derivative(const Node *root) {

}