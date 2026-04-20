#include "expression/print_tree.h"
#include "expression/node.h"
#include <stdio.h>
#include <string.h>

static char *print_tree_internal(const char *print_buf, char *buffer, const Node *root, int depth, const char *cline);

void print_tree(const Node *root) {
    char print_buf[1024 * 1024];
    print_tree_internal(print_buf, print_buf, root, 0, " --- ");
    fputs(print_buf, stdout);
}


typedef struct {
    char name[8];
} NodeName;


static int get_idx(const Node *self) {
    if (is_named_constant(self)) {
        return (int)self->value.constant.value.name;
    }
    if (is_variable(self)) {
        return NCONST_E + 1;
    }
    if (is_unary_operator(self)) {
        return NCONST_E + 2 + (int)self->value.operator.value.unary.type;
    }
    if (is_binary_operator(self)) {
        return 7 + (int)self->value.operator.value.binary.type;
    }
    return -1;
}

#define BOX_WIDTH 80
char *print_tree_internal(const char *print_buf, char *buffer, const Node *root, int depth, const char *cline) {
    NodeName names[] = {
        {"pi"},
        {"e"},
        {"x"},
        {"sin"},
        {"cos"},
        {"tan"},
        {"ctg"},
        {"+"},
        {"-"},
        {"*"},
        {"/"},
    };


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
        dst += is_imm_constant(root)
            ? sprintf(dst, "%-5.2lf", root->value.constant.value.immediate)
            : sprintf(dst, "%s", names[get_idx(root)].name);
        break;
    case NODE_VARIABLE:
        dst += sprintf(dst, "x");
        break;
    case NODE_OPERATOR:
        dst += sprintf(dst, "%s", names[get_idx(root)].name);
        break;
    default:
        break;
    }
    *dst = ' ';
    
    dst = buffer + BOX_WIDTH;
    
    if (is_operator(root) && get_argc(root) != 0) {
        for (int i = 0; i < get_argc(root) - 1; ++i) {
            dst = print_tree_internal(
                print_buf,
                dst,
                root->value.operator.value.args[i],
                depth + 1,
                "|--- "
            );
        }
        dst = print_tree_internal(
            print_buf,
            dst,
            root->value.operator.value.args[get_argc(root) - 1],
            depth + 1,
            "`--- "
        );
    }
    
    return dst;
}
