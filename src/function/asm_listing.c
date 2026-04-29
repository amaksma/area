#include "function/asm_listing.h"
#include "function/node.h"
#include "myutility.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct node_const {
    double value;
    int idx;
    struct node_const *next;
} NodeConst;

NodeConst *search_node_const(NodeConst *list, double value);
NodeConst *add_node_const(NodeConst *list, double value);
void free_list_const(NodeConst *list);

void write_expression(FILE *stream, const Node *root, NodeConst **list, const char *indent);
void write_function(FILE *stream, const Node *root, NodeConst **list, const char *name, const char *indent);
void write_constants(FILE *stream, const NodeConst *list, const char *indent);


NodeConst *search_node_const(NodeConst *list, double value) {
    if (!list) { return NULL; }
    if (list->value == value) { return list; }
    return search_node_const(list->next, value);
}

NodeConst *add_node_const(NodeConst *list, double value) {
    NodeConst *ptr = malloc(sizeof(NodeConst));
    *ptr = (NodeConst) { .value = value, .idx = list ? list->idx + 1 : 0, .next = list };
    return ptr;
}

void free_list_const(NodeConst *list) {
    if (!list) { return; }
    free_list_const(list->next);
    free(list);
}

void write_expression(FILE *stream, const Node *root, NodeConst **list, const char *indent) {
    switch (root->type) {
    case NODE_IMMEDIATE: {
        NodeConst *ptr = search_node_const(*list, root->value.immediate);
        if (!ptr) {
            ptr = *list = add_node_const(*list, root->value.immediate);
        }
        fprintf(stream, "%s" "fld qword [const%d]" " ; %.17g\n", indent, ptr->idx, ptr->value);
        break;
    }
    case NODE_VARIABLE:
        fputs(indent, stream);
        fputs("fld qword [ebp + 8]\n", stream);
        break;
    case NODE_OPERATOR:
        for (int i = 0; i < get_argc(root); ++i) {
            write_expression(stream, root->value.operator.args[i], list, indent);
        }
        const char *str = get_info(root->value.operator.type)->asm_code;
        while (*str) {
            fputs(indent, stream);
            while (*str && *str != ':') {
                fputc(*(str++), stream);
            }
            fputc('\n', stream);
            ++str;
        }
        break;
    }
}

void write_function(FILE *stream, const Node *root, NodeConst **list, const char *name, const char *indent) {
    fputs(name, stream);
    fputs(":\n", stream);
    fputs(indent, stream);
    fputs("push ebp\n", stream);
    fputs(indent, stream);
    fputs("mov ebp, esp\n", stream);
    write_expression(stream, root, list, indent);
    fputs(indent, stream);
    fputs("pop ebp\n", stream);
    fputs(indent, stream);
    fputs("ret\n", stream);
    fputc('\n', stream);
}

void write_constants(FILE *stream, const NodeConst *list, const char *indent) {
    fprintf(stream, "%s" "e: dq %#a\n", indent, C_E);
    while (list) {
        fprintf(stream, "%s" "const%d: dq %#a\n", indent, list->idx, list->value);
        list = list->next;
    }
    fputc('\n', stream);
}

void write_listing(
    FILE *stream,
    const Node *f1, const Node *f2, const Node *f3,
#ifdef NEWTON
    const Node *df1dx, const Node *df2dx, const Node *df3dx
#endif
) {
    const char indent[] = "    ";
    NodeConst *list = NULL;

    fputs("section .text\n", stream);
    write_function(stream, f1, &list, "f1", indent);
    write_function(stream, f2, &list, "f2", indent);
    write_function(stream, f3, &list, "f3", indent);

#ifdef NEWTON
    write_function(stream, df1dx, &list, "df1dx", indent);
    write_function(stream, df2dx, &list, "df2dx", indent);
    write_function(stream, df3dx, &list, "df3dx", indent);
#endif

    fputs("section .rodata\n", stream);
    write_constants(stream, list, indent);

    free_list_const(list);
}


