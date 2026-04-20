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

static const OperatorInfo op_info[] = {
    (OperatorInfo) {0, "pi",  "0",                      "fldpi"              },
    (OperatorInfo) {0, "e",   "0",                      "fld qword [e]"      },
    (OperatorInfo) {1, "sin", "f F cos *",              "fsin"               },
    (OperatorInfo) {1, "cos", "f -1 * F sin *",         "fcos"               },
    (OperatorInfo) {1, "tan", "f F cos F cos * /",      "fptan:fstp st0"     },
    (OperatorInfo) {1, "ctg", "f -1 * F sin F sin * /", "fptan:fdivrp"       },
    (OperatorInfo) {2, "+",   "f g +",                  "faddp"              },
    (OperatorInfo) {2, "-",   "f g -",                  "fsubp"              },
    (OperatorInfo) {2, "*",   "f G * F g * +",          "fmulp"              },
    (OperatorInfo) {2, "/",   "f G / F g * G G * / -",  "fdivp"              },
};

// Get the information about the operator
const OperatorInfo *get_info(OperatorType type);

bool is_constant(const Node *self);
bool is_immediate(const Node *self);
bool is_variable(const Node *self);
bool is_operator(const Node *self);
int get_argc(const Node *self);

Node *new(Node node);
Node *deep_copy(const Node *root);
Node from_immediate(double value);
Node from_variable(char variable);
Node from_operator(OperatorType type);

Node *substitute_variable(Node *root, const Node *src, char variable);

#endif
