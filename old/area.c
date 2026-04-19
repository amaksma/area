#include <stdio.h>

#include "function_parser.h"

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
