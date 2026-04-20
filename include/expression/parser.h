#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include "expression/node.h"


// Constructs a function tree from the input string
// by parsing an expression written in a reverse Polish notation
Node *tree_from_string(const char *str);

// Constructs a node from a string
Node from_string(const char *str);

#endif
