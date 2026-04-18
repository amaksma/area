#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#define CONST_PI 3.141592653589793238
#define CONST_E  2.718281828459045235



typedef enum {
    OP_UNDEFINED,
    // Variable
    OP_X,
    // Constants
    OP_PI,
    OP_E,
    // Unary operators
//    OP_SQRT,
//    OP_EXP,
//    OP_LN,
    OP_SIN,
    OP_COS,
    OP_TAN,
    OP_CTG,
    // Binary operators
//    OP_POW,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
} OperatorType;
#define NUM_OPERATORS (OP_DIV + 1)

typedef struct {
    int arg_count;
    char name[12];
    char cmd[128];
} OperatorInfo;

static const OperatorInfo op_info[] = {
    (OperatorInfo) {0, "????", "????"},
    (OperatorInfo) {0, "x",    "fld qword [ebp + 8]"},
    (OperatorInfo) {0, "pi",   "fldpi"},
    (OperatorInfo) {0, "e",    "fld qword [e]"},
//    (OperatorInfo) {1, "sqrt", "fsqrt"},
//    (OperatorInfo) {1, "exp",  "fldl2e:fmulp:fld st0:frndint:fsub st1, st0:fxch:f2xm1:fld1:faddp:fscale:fstp st1"},
//    (OperatorInfo) {1, "ln",   "fld1:fldl2e:fdivp:fxch:fyl2x"},
    (OperatorInfo) {1, "sin",  "fsin"},
    (OperatorInfo) {1, "cos",  "fcos"},
    (OperatorInfo) {1, "tan",  "fptan:fstp st0"},
    (OperatorInfo) {1, "ctg",  "fptan:fdivrp"},
//    (OperatorInfo) {2, "pow",  "fyl2x:fld st0:frndint:fsub st1, st0:fxch:f2xm1:fld1:faddp:fscale:fstp st1"},
    (OperatorInfo) {2, "+",    "faddp"},
    (OperatorInfo) {2, "-",    "fsubp"},
    (OperatorInfo) {2, "*",    "fmulp"},
    (OperatorInfo) {2, "/",    "fdivp"},
};

// Get the information about the operator
static const OperatorInfo *get_info(OperatorType type);
// Get the name of the operator
static const char *get_name(OperatorType type);
// Get the number of arguments of the operator
static int get_argc(OperatorType type);


typedef enum {
    NODE_UNDEFINED,
    NODE_CONSTANT,
    NODE_VARIABLE,
    NODE_OPERATOR
} NodeType;

typedef struct node {
    NodeType type;
    union {
        // Constant
        double value;
        // Operator
        struct {
        OperatorType op_type;
        union {
            struct node *args[2];
            // Unary
            struct node *arg;
            // Binary
            struct {
                struct node *left;
                struct node *right;
            };
        };
        };
    };
} Node;

typedef struct {
    Node *buffer;
    Node *root;
} Tree;

static bool is_constant(const Node *self);
static bool is_variable(const Node *self);
static bool is_operator(const Node *self);

static Node *new();
static Node from_constant(double value);
static Node from_variable();
static Node from_operator(OperatorType type);
static Node from_string(const char *str);

static Node *deep_copy(const Node *root);


// Constructs a function tree from the input string
// by parsing an expression written in a reverse Polish notation
static Tree construct_tree(char *str);
static void destruct_tree(Tree *tree);


// Support function for the 'construct_tree(char *) -> Tree' function
static Node *link_tree(Node **spp, const Node *bottom);
static size_t count_chars(const char *str, char ch);

static void print_tree(const Tree *tree);
static char *print_tree_internal(const char *print_buf, char *buffer, const Node *root, int depth, const char *cline);



int main(void) {
    printf("%s ", get_name(OP_SIN));
    printf("%d\n", get_argc(OP_SIN));
    
    //char str[] = "1 2 +";
    //Tree func = construct_tree(str);
    
    // x - (xx - 5) / 1000 + sin(xx) - 2
    char str[] = "x x x * 5 - 1000 / - x x * sin + 2 -";
    Tree func = construct_tree(str);
    print_tree(&func);
    
    destruct_tree(&func);
    
    return 0;
}







const OperatorInfo *get_info(OperatorType type) {
    assert(NUM_OPERATORS == sizeof(op_info) / sizeof(*op_info));
    assert(type >= OP_UNDEFINED && type < NUM_OPERATORS);
    return &op_info[type];
}

const char *get_name(OperatorType type) {
    return get_info(type)->name;
}

int get_argc(OperatorType type) {
    return get_info(type)->arg_count;
}





bool is_constant(const Node *self) {
    return self->type == NODE_CONSTANT;
}

bool is_variable(const Node *self) {
    return self->type == NODE_VARIABLE;
}

bool is_operator(const Node *self) {
    return self->type == NODE_OPERATOR;
}


Node *new() {
    return malloc(sizeof(Node));
}

Node from_constant(double value) {
    return (Node) {
        .type = NODE_CONSTANT,
        .value = value
    };
}

Node from_variable() {
    return (Node) {
        .type = NODE_VARIABLE
    };
}

Node from_operator(OperatorType type) {
    return (Node) {
        .type = NODE_OPERATOR,
        .op_type = type
    };
}

Node from_string(const char *str) {
    assert(str);
    double value;
    if (sscanf(str, "%lf", &value) == 1) {
        return from_constant(value);
    }
    
    if (strcmp(str, "x") == 0) {
        return from_variable();
    }
    
    for (OperatorType type = 1; type < NUM_OPERATORS; ++type) {
        if (strcmp(str, get_name(type)) == 0) {
            return from_operator(type);
        }
    }
    printf("Invalid name: %s\n", str);
    assert(0 && "Unreachable");
}


Node *deep_copy(const Node *root) {
    assert(root);
    Node *copy = new();
    *copy = *root;
    if (is_operator(copy)) {
        for (int i = 0; i < get_argc(copy->op_type); ++i) {
            copy->args[i] = deep_copy(copy->args[i]);
        }
    }
    return copy;
}





Tree construct_tree(char *str) {
    assert(str);
    size_t tokens = count_chars(str, ' ') + 1;
    Node *stack = malloc(sizeof(Node) * tokens);
    Node *bottom = stack + tokens;
    Node *sp = bottom; // One past end
    
    char *token = strtok(str, " ");
    while (token) {
        assert(sp != stack);
        *(--sp) = from_string(token); // push
        token = strtok(NULL, " ");
    }
    
    return (Tree) {
        .buffer = stack,
        .root = link_tree(&sp, bottom)
    };
}

void destruct_tree(Tree *tree) {
    free(tree->buffer);
}


Node *link_tree(Node **spp, const Node *bottom) {
    assert(spp && *spp && *spp != bottom);
    Node *root = (*spp)++; // pop
    if (is_operator(root)) {
        int count = get_argc(root->op_type);
        for (int i = count; i-- > 0; ) {
            root->args[i] = link_tree(spp, bottom);
        }
    }
    return root;
}




size_t count_chars(const char *str, char ch) {
    size_t count = 0;
    while (*str) {
        if (*str == ch) { ++count; }
        ++str;
    }
    return count;
}




void print_tree(const Tree *tree) {
    char print_buf[1024 * 1024];
    print_tree_internal(print_buf, print_buf, tree->root, 0, " --- ");
    fputs(print_buf, stdout);
}

#define BOX_WIDTH 50
char *print_tree_internal(const char *print_buf, char *buffer, const Node *root, int depth, const char *cline) {
    if (!root) {
        buffer[0] = 0;
        return buffer;
    }
    
    memset(buffer, ' ', BOX_WIDTH - 1);
    buffer[BOX_WIDTH - 1] = '\n';
    buffer[BOX_WIDTH] = 0;
    
    char *dst = buffer;
    for (int i = 0; i < depth; ++i) {
        char *src = dst - BOX_WIDTH;
        char vline[] = "|    ";
        if (src < print_buf
            || *src == ' '
            || *src == '`') {
            *vline = ' ';
        }
        
        dst += sprintf(dst, "%5s", vline);
    }
    
    dst += sprintf(dst, "%5s", cline);
    switch (root->type) {
    case NODE_CONSTANT:
        dst += sprintf(dst, "%-5.2lf", root->value);
        break;
    case NODE_VARIABLE:
        dst += sprintf(dst, "x");
        break;
    case NODE_OPERATOR:
        dst += sprintf(dst, "%s", get_name(root->op_type));
        break;
    default:
        break;
    }
    *dst = ' ';
    
    dst = buffer + BOX_WIDTH;
    
    if (is_operator(root) && get_argc(root->op_type) != 0) {
        for (int i = 0; i < get_argc(root->op_type) - 1; ++i) {
            dst = print_tree_internal(
                print_buf,
                dst,
                root->args[i],
                depth + 1,
                "|--- "
            );
        }
        dst = print_tree_internal(
            print_buf,
            dst,
            root->args[get_argc(root->op_type) - 1],
            depth + 1,
            "`--- "
        );
    }
    
    return dst;
}

