#include "function/node.h"
#include "function/parser.h"
#include "function/print_tree.h"

int main(void) {
    Node *root = construct_tree("x x x * 5 - 1000 / - x x * sin + 2 -");
    print_tree(root);
    destruct_tree(root);

    root = construct_tree(get_info(OP_CTG)->derivative);
    print_tree(root);
    destruct_tree(root);

    return 0;
}
