#include "print_tree.h"
#include <stdio.h>
#include <string.h>
#include "function_parser.h"

static char *print_tree_internal(const char *print_buf, char *buffer, const Node *root, int depth, const char *cline);

void print_tree(const Node *root) {
    char print_buf[1024 * 1024];
    print_tree_internal(print_buf, print_buf, root, 0, " --- ");
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
    case NODE_IMMEDIATE:
        dst += sprintf(dst, "%-5.2lf", root->value.immediate);
        break;
    case NODE_VARIABLE:
        dst += sprintf(dst, "x");
        break;
    case NODE_OPERATOR:
        dst += sprintf(dst, "%s", get_info(root->value.operator.type)->name);
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
                root->value.operator.args[i],
                depth + 1,
                "|--- "
            );
        }
        dst = print_tree_internal(
            print_buf,
            dst,
            root->value.operator.args[get_argc(root) - 1],
            depth + 1,
            "`--- "
        );
    }
    
    return dst;
}
