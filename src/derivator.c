#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#define C_PI 3.141592653589793238
#define C_E  2.718281828459045235


enum operators {
    OP_UNDEFINED,
    OP_UNARY_SQRT,
    OP_UNARY_EXP,
    OP_UNARY_LN,
    OP_UNARY_SIN,
    OP_UNARY_COS,
    OP_UNARY_TAN,
    OP_UNARY_CTG,
    OP_BINARY_ADD,
    OP_BINARY_SUB,
    OP_BINARY_MUL,
    OP_BINARY_DIV,
    NUM_OPERANDS
};

struct op_data {
    enum operators type;
    int argc;
    char str[8];
};

static const struct op_data op_data[] = {
    (struct op_data) {OP_UNDEFINED,  0, "????"},
    (struct op_data) {OP_UNARY_SQRT, 1, "sqrt"},
    (struct op_data) {OP_UNARY_EXP,  1, "exp"},
    (struct op_data) {OP_UNARY_LN,   1, "ln"},
    (struct op_data) {OP_UNARY_SIN,  1, "sin"},
    (struct op_data) {OP_UNARY_COS,  1, "cos"},
    (struct op_data) {OP_UNARY_TAN,  1, "tan"},
    (struct op_data) {OP_UNARY_CTG,  1, "ctg"},
    (struct op_data) {OP_BINARY_ADD, 2, "+"},
    (struct op_data) {OP_BINARY_SUB, 2, "-"},
    (struct op_data) {OP_BINARY_MUL, 2, "*"},
    (struct op_data) {OP_BINARY_DIV, 2, "/"},
};
#define OP_DATA_SIZE (sizeof(op_data) / sizeof(*op_data))





struct node {
    enum {
        NODE_UNDEFINED,
        NODE_CONSTANT,
        NODE_VARIABLE,
        NODE_OPERATOR
    } type;
    
    union {
        // Constant
        double value;
        // Operator
        struct {
            enum operators op_type;
            int argc;
            union {
                struct node *args[2];
                struct node *arg;
                struct {
                    struct node *right;
                    struct node *left;
                };
            };
        };
    };
};

static bool is_operator(const struct node *self) {
    return self->type == NODE_OPERATOR;
}

static bool is_constant(const struct node *self) {
    return self->type == NODE_CONSTANT;
}

static bool is_variable(const struct node *self) {
    return self->type == NODE_VARIABLE;
}

static struct node *new() {
    struct node *ptr = malloc(sizeof(struct node));
    *ptr = (struct node) { .type = NODE_UNDEFINED };
    return ptr;
}

static struct node *from_constant(double value) {
    struct node *ptr = new();
    *ptr = (struct node) {
        .type = NODE_CONSTANT,
        .value = value
    };
    return ptr;
}

static struct node *from_variable() {
    struct node *ptr = new();
    *ptr = (struct node) {
        .type = NODE_VARIABLE,
    };
    return ptr;
}

static struct node *from_op(enum operators type, int arg_count) {
    assert(arg_count == 1 || arg_count == 2);
    struct node *ptr = new();
    *ptr = (struct node) {
        .type = NODE_OPERATOR,
        .op_type = type,
        .argc = arg_count
    };
    return ptr;
}

static struct node *parse_token(const char *token) {
    assert(token && token[0] != 0);
    
    double constant;
    if (sscanf(token, "%lf", &constant) == 1) {
        return from_constant(constant);
    }
    if (strcmp(token, "pi") == 0) {
        return from_constant(C_PI);
    }
    if (strcmp(token, "e") == 0) {
        return from_constant(C_E);
    }
    if (strcmp(token, "x") == 0) {
        return from_variable();
    }
    
    for (size_t i = 0; i < OP_DATA_SIZE; ++i) {
        if (strcmp(token, op_data[i].str) != 0) { continue; }
        return from_op(op_data[i].type, op_data[i].argc);
    }
    
    assert(false && "Invalid token");
}

// Links the nodes from the stack into a tree
// The nodes are in a revese Polish notation
static struct node *link_tree(struct node ***spp) {
    assert(spp && *spp && **spp);
    struct node *root = *((*spp)++); // pop the top
    if (is_operator(root)) {
        for (int i = 0; i < root->argc; ++i) {
            root->args[i] = link_tree(spp);
        }
    }
    return root;
}

#define STACK_SIZE 1024
// Creates a function tree from an expression in a reverse Polish notation
static struct node *build_tree(char *str) {
    struct node *bottom[STACK_SIZE];
    struct node **sp = bottom + STACK_SIZE; // one past end
    
    char *token = strtok(str, " ");
    while (token) {
        assert(sp != bottom);
        *--sp = parse_token(token);
        token = strtok(NULL, " ");
    }
    
    return link_tree(&sp);
}

// Returns the root of a deep copy of the tree
static struct node *copy_tree(const struct node *root) {
    if (!root) { return NULL; }
    struct node *ptr = new();
    *ptr = *root;
    if (is_operator(root)) {
        for (int i = 0; i < root->argc; ++i) {
            ptr->args[i] = copy_tree(root->args[i]);
        }
    }
    return ptr;
}

// Deletes all nodes of the tree
static void delete_tree(struct node *root) {
    if (is_operator(root)) {
        for (int i = 0; i < root->argc; ++i) {
            delete_tree(root->args[i]);
        }
    }
    free(root);
}

static double invoke_unary(enum operators op, double x) {
    switch (op) {
    case OP_UNARY_SQRT:
        return sqrt(x);
    case OP_UNARY_EXP:
        return exp(x);
    case OP_UNARY_LN:
        return log(x);
    case OP_UNARY_SIN:
        return sin(x);
    case OP_UNARY_COS:
        return cos(x);
    case OP_UNARY_TAN:
        return tan(x);
    case OP_UNARY_CTG:
        return 1.0 / tan(x);
    default:
        abort();
        break;
    }
}

static double invoke_binary(enum operators op, double left, double right) {
    switch (op) {
    case OP_BINARY_ADD:
        return left + right;
    case OP_BINARY_SUB:
        return left - right;
    case OP_BINARY_MUL:
        return left * right;
    case OP_BINARY_DIV:
        return left / right;
    default:
        abort();
        break;
    }
}

static struct node *simplify_unary_op(struct node *self) {
    assert(self->argc == 1);
    struct node *arg = self->arg;
    
    // Simplify the exp(ln(...)) and ln(exp(...))
    if (is_operator(arg)
        && self->op_type == OP_UNARY_EXP && arg->op_type == OP_UNARY_LN
        && self->op_type == OP_UNARY_LN && arg->op_type == OP_UNARY_EXP
    ) {
        struct node *arg_arg = arg->arg;
        free(arg);
        free(self);
        return arg_arg;
    }
    if (!is_constant(arg)) { return self; }
    
    double value = arg->value;
    free(arg);
    *self = (struct node) {
        .type = NODE_CONSTANT,
        .value = invoke_unary(self->op_type, value)
    };
    
    return self;
}

static struct node *simplify_binary_op(struct node *self) {
    assert(self->argc == 2);
    struct node *right = self->right;
    struct node *left = self->left;
    if (is_constant(left) && is_constant(right)) {
        double value = invoke_binary(self->op_type, left->value, right->value);
        free(left);
        free(right);
        *self = (struct node) {
            .type = NODE_CONSTANT,
            .value = value
        };
    }
    switch (self->op_type) {
    case OP_BINARY_ADD:
        if (is_constant(left) && left->value == 0.0) {
            free(left);
            free(self);
            return right;
        }
        if (is_constant(right) && right->value == 0.0) {
            free(right);
            free(self);
            return left;
        }
        break;
    case OP_BINARY_SUB:
        if (is_constant(right) && right->value == 0.0) {
            free(right);
            free(self);
            return left;
        }
        if (is_variable(right) && is_variable(left)) {
            free(right);
            free(left);
            *self = (struct node) {
                .type = NODE_CONSTANT,
                .value = 0.0
            };
            return self;
        }
        break;
    case OP_BINARY_MUL:
        if (is_constant(left) && left->value == 1.0) {
            free(left);
            free(self);
            return right;
        }
        if (is_constant(right) && right->value == 1.0) {
            free(right);
            free(self);
            return left;
        }
        if (is_constant(left) && left->value == 0.0) {
            free(self);
            delete_tree(right);
            return left;
        }
        if (is_constant(right) && right->value == 0.0) {
            free(self);
            delete_tree(left);
            return right;
        }
        break;
    case OP_BINARY_DIV:
        if (is_constant(right) && right->value == 1.0) {
            free(right);
            free(self);
            return left;
        }
        if (is_constant(left) && left->value == 0.0) {
            free(self);
            delete_tree(right);
            return left;
        }
        if (is_variable(right) && is_variable(left)) {
            free(right);
            free(left);
            *self = (struct node) {
                .type = NODE_CONSTANT,
                .value = 1.0
            };
            return self;
        }
        break;
    default:
        break;
    }
    return self;
}

static struct node *simplify(struct node *root) {
    switch (root->type) {
    case NODE_OPERATOR:
        for (int i = 0; i < root->argc; ++i) {
            root->args[i] = simplify(root->args[i]);
        }
        if (root->argc == 1) {
            return simplify_unary_op(root);
        }
        if (root->argc == 2) {
            return simplify_binary_op(root);
        }
        break;
    default:
        break;
    }
    return root;
}

static struct node *derive(struct node *root);
static struct node *derive_op(struct node *root) {
    assert(is_operator(root));
    
    switch (root->op_type) {
    case OP_UNARY_SQRT: {
        struct node *mulp = from_op(OP_BINARY_MUL, 2);
        struct node *divp = from_op(OP_BINARY_DIV, 2);
        mulp->right = derive(copy_tree(root->arg));
        mulp->left = divp;
        divp->right = root;
        divp->left = from_constant(0.5);
        return mulp;
    }
    case OP_UNARY_EXP: {
        struct node *mulp = from_op(OP_BINARY_MUL, 2);
        mulp->right = derive(copy_tree(root->arg));
        mulp->left = root;
        return mulp;
    }
    case OP_UNARY_LN: {
        struct node *divp = from_op(OP_BINARY_DIV, 2);
        divp->right = root->arg;
        divp->left = derive(copy_tree(root->arg));
        return divp;
    }
    case OP_UNARY_SIN: {
        struct node *mulp = from_op(OP_BINARY_MUL, 2);
        mulp->right = derive(copy_tree(root->arg));
        mulp->left = root;
        root->op_type = OP_UNARY_COS;
        return mulp;
    }
    case OP_UNARY_COS: {
        struct node *mulp = from_op(OP_BINARY_MUL, 2);
        struct node *mulp_neg = from_op(OP_BINARY_MUL, 2);
        mulp->right = derive(copy_tree(root->arg));
        mulp->left = mulp_neg;
        mulp_neg->right = root;
        mulp_neg->left = from_constant(-1.0);
        root->op_type = OP_UNARY_SIN;
        return mulp;
    }
    case OP_UNARY_TAN: {
        struct node *divp = from_op(OP_BINARY_DIV, 2);
        struct node *mulp = from_op(OP_BINARY_MUL, 2);
        divp->right = mulp;
        divp->left = derive(copy_tree(root->arg));
        root->op_type = OP_UNARY_COS;
        mulp->right = root;
        mulp->left = copy_tree(root);
        return divp;
    }
    case OP_UNARY_CTG: {
        struct node *divp = from_op(OP_BINARY_DIV, 2);
        struct node *mulp = from_op(OP_BINARY_MUL, 2);
        struct node *mulp_neg = from_op(OP_BINARY_MUL, 2);
        mulp_neg->right = derive(copy_tree(root->arg));
        mulp_neg->left = from_constant(-1.0);
        divp->right = mulp;
        divp->left = mulp_neg;
        root->op_type = OP_UNARY_SIN;
        mulp->right = root;
        mulp->left = copy_tree(root);
        return divp;
    }
    case OP_BINARY_ADD:
    case OP_BINARY_SUB:
        root->left = derive(root->left);
        root->right = derive(root->right);
        return root;
    case OP_BINARY_MUL: {
        struct node *mulp1 = from_op(OP_BINARY_MUL, 2);
        struct node *mulp2 = from_op(OP_BINARY_MUL, 2);
        mulp1->right = root->right;
        mulp1->left = derive(copy_tree(root->left));
        mulp2->right = derive(copy_tree(root->right));
        mulp2->left = root->left;
        root->op_type = OP_BINARY_ADD;
        root->right = mulp2;
        root->left = mulp1;
        return root;
    }
    case OP_BINARY_DIV: {
        struct node *mulp1 = from_op(OP_BINARY_MUL, 2);
        struct node *mulp2 = from_op(OP_BINARY_MUL, 2);
        mulp1->right = root->right;
        mulp1->left = derive(copy_tree(root->left));
        mulp2->right = derive(copy_tree(root->right));
        mulp2->left = root->left;
        struct node *subp = from_op(OP_BINARY_SUB, 2);
        subp->right = mulp2;
        subp->left = mulp1;
        struct node *mulp3 = from_op(OP_BINARY_MUL, 2);
        mulp3->right = copy_tree(root->right);
        mulp3->left = copy_tree(root->right);
        root->left = subp;
        root->right = mulp3;
        return root;
    }
    default:
        break;
    }
}

static struct node *derive(struct node *root) {
    switch (root->type) {
    case NODE_CONSTANT:
        root->value = 0;
        return root;
    case NODE_VARIABLE:
        *root = (struct node) {
            .type = NODE_CONSTANT,
            .value = 1.0
        };
        return root;
    case NODE_OPERATOR:
        return root = derive_op(root);
    default:
        abort();
    }
}


static const char *node_data_str(const struct node *root) {
    return op_data[root->op_type].str;
}

#define BOX_WIDTH 50
static char print_buf[1024 * 1024];
static char *print_tree(char *buffer, struct node *root, int depth, const char *cline) {
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
        dst += sprintf(dst, "%s", node_data_str(root));
        break;
    default:
        break;
    }
    *dst = ' ';
    
    dst = buffer + BOX_WIDTH;
    
    if (is_operator(root) && root->argc != 0) {
        for (int i = 0; i < root->argc - 1; ++i) {
            dst = print_tree(
                dst,
                root->args[i],
                depth + 1,
                "|--- "
            );
        }
        dst = print_tree(
            dst,
            root->args[root->argc - 1],
            depth + 1,
            "`--- "
        );
    }
    
    return dst;
}


// exp(x) - exp(x**2 - 5) / 1000 + sin(x**2) - 2
// exp(x) - 2x * exp(x**2 - 5) / 1000 + 2x * sin(x**2)

// x exp x x * 5 - exp 1000 / - x x * sin 2 -

int main(void) {
    assert(NUM_OPERANDS == OP_DATA_SIZE);
    //char *str = "x x +";
    //struct node *root = build_tree(str);
    //const char *token = "exp";
    //struct node *root = parse_token(token);
    //root->arg = parse_token("pi");
    
    char str[] = "2 x * x * 3 pi sin * - exp 5 +";
    //char str1[] = "x 2 x * * 3 - exp 5 +";
    char str2[] = "x x * tan";
    char str3[] = "x exp x x * 5 - exp 1000 / - x x * sin + 2 -";
    struct node *root = build_tree(str3);
    print_tree(print_buf, root, 0, " --- ");
    printf("%s", print_buf);
    //root = simplify(root);
    //print_tree(print_buf, root, 0, " --- ");
    //printf("%s", print_buf);
    root = derive(root);
    root = simplify(root);
    print_tree(print_buf, root, 0, " --- ");
    printf("%s", print_buf);
    /*struct node *root1 = copy_tree(root);
    root1->op_type = OP_BINARY_SUB;
    print_tree(print_buf, root1, 0, " --- ");
    printf("%s", print_buf);*/
    delete_tree(root);
    
    return 0;
}