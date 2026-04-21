#ifndef NODE_H
#define NODE_H

#include <stdbool.h>

#define MAX_ARGS 2


typedef enum {
    NODE_IMMEDIATE,
    NODE_VARIABLE,
    NODE_OPERATOR
} NodeType;

typedef enum {
    // Constants
    OP_PI,
    OP_E,
    // Unary operators
    OP_SIN,
    OP_COS,
    OP_TAN,
    OP_CTG,
    // Binary operators
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
} OperatorType;
#define NUM_OPERATORS ((size_t)(OP_DIV + 1))


// Expression node
typedef struct node {
    NodeType type;
    union {
        double immediate;
        char variable;
        struct {
            OperatorType type;
            struct node *args[MAX_ARGS];
        } operator;
    } value;
} Node;

typedef struct {
    int argc;
    char name[128];
    char derivative[128];
    char asm_code[128];
} OperatorInfo;

// Information about the operators
extern const OperatorInfo op_info[];

// Get the information about the operator
const OperatorInfo *get_info(OperatorType type);

// Checks if the node is an immediate value or a zero-operand operator
bool is_constant(const Node *self);
// Checks if the node is an immediate value
bool is_immediate(const Node *self);
// Checks if the node is a variable
bool is_variable(const Node *self);
// Checks if the node is an operator
bool is_operator(const Node *self);
// Get the number of operands
int get_argc(const Node *self);

Node *new(Node node);
Node *deep_copy(const Node *root);
Node from_immediate(double value);
Node from_variable(char variable);
Node from_operator(OperatorType type);

Node *substitute_variable(Node *root, const Node *src, char variable);

#endif
