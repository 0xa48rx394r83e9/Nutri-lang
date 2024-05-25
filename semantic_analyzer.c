#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic_analyzer.h"
#include "ast.h"

// Symbol table entry
typedef struct
{
    char *name;
    int type;
} SymbolEntry;

// Symbol table
SymbolEntry *symbol_table = NULL;
int symbol_count = 0;

// Function to add a symbol to the symbol table
void add_symbol(char *name, int type)
{
    for (int i = 0; i < symbol_count; i++)
    {
        if (strcmp(symbol_table[i].name, name) == 0)
        {
            fprintf(stderr, "Redeclaration of variable: %s\n", name);
            exit(1);
        }
    }
    symbol_table = (SymbolEntry *)realloc(symbol_table, (symbol_count + 1) * sizeof(SymbolEntry));
    symbol_table[symbol_count].name = strdup(name);
    symbol_table[symbol_count].type = type;
    symbol_count++;
}

// Function to look up a symbol in the symbol table
int lookup_symbol(char *name)
{
    for (int i = 0; i < symbol_count; i++)
    {
        if (strcmp(symbol_table[i].name, name) == 0)
        {
            return symbol_table[i].type;
        }
    }
    return -1;
}

// Semantic analysis recursive function
void semantic_analysis_recursive(AST *node)
{
    if (node == NULL)
    {
        return;
    }

    switch (node->type)
    {
    case AST_VARIABLE_DECLARATION:
    {
        add_symbol(node->variable_declaration_name, node->variable_declaration_value->type);
        semantic_analysis_recursive(node->variable_declaration_value);
        break;
    }
    case AST_BINARY_EXPRESSION:
    {
        semantic_analysis_recursive(node->binary_left);
        semantic_analysis_recursive(node->binary_right);
        if (node->binary_left->type != node->binary_right->type)
        {
            fprintf(stderr, "Type mismatch in binary expression\n");
            exit(1);
        }
        node->type = node->binary_left->type;
        break;
    }
    case AST_IDENTIFIER:
    {
        int type = lookup_symbol(node->identifier_name);
        if (type == -1)
        {
            fprintf(stderr, "Undeclared variable: %s\n", node->identifier_name);
            exit(1);
        }
        node->type = type;
        break;
    }
    case AST_FUNCTION_CALL:
    {
        // Perform semantic analysis on function call arguments
        for (int i = 0; i < node->function_call_argument_count; i++)
        {
            semantic_analysis_recursive(&node->function_call_arguments[i]);
        }
        break;
    }
    case AST_IF_STATEMENT:
    {
        semantic_analysis_recursive(node->if_condition);
        if (node->if_condition->type != AST_INTEGER)
        {
            fprintf(stderr, "If condition must be an integer\n");
            exit(1);
        }
        semantic_analysis_recursive(node->if_body);
        if (node->if_else != NULL)
        {
            semantic_analysis_recursive(node->if_else);
        }
        break;
    }
    case AST_WHILE_STATEMENT:
    {
        semantic_analysis_recursive(node->while_condition);
        if (node->while_condition->type != AST_INTEGER)
        {
            fprintf(stderr, "While condition must be an integer\n");
            exit(1);
        }
        semantic_analysis_recursive(node->while_body);
        break;
    }
    case AST_FOR_STATEMENT:
    {
        semantic_analysis_recursive(node->for_init);
        semantic_analysis_recursive(node->for_condition);
        if (node->for_condition->type != AST_INTEGER)
        {
            fprintf(stderr, "For condition must be an integer\n");
            exit(1);
        }
        semantic_analysis_recursive(node->for_increment);
        semantic_analysis_recursive(node->for_body);
        break;
    }
    case AST_RETURN_STATEMENT:
    {
        semantic_analysis_recursive(node->return_value);
        break;
    }
    case AST_BLOCK:
    {
        for (int i = 0; i < node->block_statement_count; i++)
        {
            semantic_analysis_recursive(node->block_statements[i]);
        }
        break;
    }
    case AST_EXPRESSION_STATEMENT:
    {
        semantic_analysis_recursive(node->expression_statement);
        break;
    }
    case AST_INTEGER:
    case AST_FLOAT:
        break;
    default:
        fprintf(stderr, "Unknown AST node type: %d\n", node->type);
        exit(1);
    }
}

// Semantic analysis function
void semantic_analysis(AST *ast)
{
    symbol_table = NULL;
    symbol_count = 0;
    semantic_analysis_recursive(ast);
}