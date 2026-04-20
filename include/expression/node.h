#ifndef EXPRESSION_NODE_H
#define EXPRESSION_NODE_H

#include <stdbool.h>


typedef enum {
    NODE_CONSTANT,
    NODE_VARIABLE,
    NODE_OPERATOR
} NodeType;


typedef enum {
    CONST_IMMEDIATE,
    CONST_NAMED
} ConstantType;

typedef enum {
    NCONST_PI,
    NCONST_E
} NamedConstant;


typedef enum {
    OP_UNARY,
    OP_BINARY
} OperatorType;

typedef enum {
    UOP_SIN,
    UOP_COS,
    UOP_TAN,
    UOP_CTG
} UnaryOperator;

typedef enum {
    BOP_ADD,
    BOP_SUB,
    BOP_MUL,
    BOP_DIV
} BinaryOperator;

typedef struct node {
    NodeType type;
    union {

        struct {
            ConstantType type;
            union {
                double immediate;
                NamedConstant name;
            } value;
        } constant;

        struct {
            OperatorType type;
            union {
                struct node *args[2];
                struct {
                    struct node *arg;
                    UnaryOperator type;
                } unary;
                struct {
                    struct node *left;
                    struct node *right;
                    BinaryOperator type;
                } binary;
            } value;
        } operator;

    } value;
} Node;


bool is_constant(const Node *self);
bool is_variable(const Node *self);
bool is_operator(const Node *self);

bool is_imm_constant(const Node *self);
bool is_named_constant(const Node *self);
bool is_unary_operator(const Node *self);
bool is_binary_operator(const Node *self);

int get_argc(const Node *self);

Node *new(Node node);
Node *deep_copy(const Node *root);
void deep_free(Node *root);

Node from_immediate(double value);
Node from_named(NamedConstant name) ;
Node from_variable(void);
Node from_unary_operator(UnaryOperator type);
Node from_binary_operator(BinaryOperator type);
Node from_unary_operator_args(UnaryOperator type, Node *arg);
Node from_binary_operator_args(BinaryOperator type, Node *left, Node *right);


#endif
