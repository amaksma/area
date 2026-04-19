#include "function_parser.h"
#include "print_tree.h"

int main(void) {
    Node *root = construct_tree("x x x * 5 - 1000 / - x x * sin + 2 -");
    print_tree(root);
    destruct_tree(root);

    return 0;
}
