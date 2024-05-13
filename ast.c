#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

// Function to create a new AST node
AST *create_ast_node(ASTNodeType type)
{
    AST *node = (AST *)malloc(sizeof(AST));
    node->type = type;
    return node;
}