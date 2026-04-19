#ifndef FUNCTION_PARSER_H
#define FUNCTION_PARSER_H

#include <stdbool.h>
#include <stdint.h>
#include "function/node.h"


// Constructs a function tree from the input string
// by parsing an expression written in a reverse Polish notation
Node *construct_tree(const char *str);
// Frees the nodes
void destruct_tree(Node *root);

// Constructs a node from a string
Node *from_string(const char *src);

#endif
