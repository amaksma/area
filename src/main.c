#include "function/derivative.h"
#include "function/node.h"
#include "function/parser.h"
#include "function/print_tree.h"
#include "function/simplify.h"

int main(void) {
    Node *root = construct_tree("x x x * 5 - 1000 / - x x * sin + 2 -");
    print_tree(root);

    Node *droot = derivative(root, 'x');

    print_tree(droot);
    droot = simplify(droot);
    print_tree(droot);
    destruct_tree(droot);
    destruct_tree(root);

    return 0;
}
