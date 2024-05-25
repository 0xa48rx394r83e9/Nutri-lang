#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "semantic_analyzer.h"
#include "interpreter.h"

// Symbol table entry
typedef struct
{
    char *name;
    int type;
} SymbolEntry;

// Symbol table
SymbolEntry *symbol_table = NULL;
int symbol_count = 0;

// Variable entry
typedef struct
{
    char *name;
    int type;
    union
    {
        int integer_value;
        float float_value;
    };
} VariableEntry;

// Variable table
VariableEntry *variable_table = NULL;
int variable_count = 0;

// Function to free the AST
void free_ast(AST *ast)
{
    if (ast == NULL)
    {
        return;
    }

    switch (ast->type)
    {
    case AST_VARIABLE_DECLARATION:
        free(ast->variable_declaration_name);
        free_ast(ast->variable_declaration_value);
        break;
    case AST_BINARY_EXPRESSION:
        free_ast(ast->binary_left);
        free_ast(ast->binary_right);
        break;
    case AST_INTEGER:
    case AST_FLOAT:
        // No need to free anything
        break;
    case AST_IDENTIFIER:
        free(ast->identifier_name);
        break;
    case AST_FUNCTION_CALL:
        free(ast->function_call_name);
        for (int i = 0; i < ast->function_call_argument_count; i++)
        {
            free_ast(&ast->function_call_arguments[i]);
        }
        free(ast->function_call_arguments);
        break;
    case AST_IF_STATEMENT:
        free_ast(ast->if_condition);
        free_ast(ast->if_body);
        if (ast->if_else != NULL)
        {
            free_ast(ast->if_else);
        }
        break;
    case AST_WHILE_STATEMENT:
        free_ast(ast->while_condition);
        free_ast(ast->while_body);
        break;
    case AST_FOR_STATEMENT:
        free_ast(ast->for_init);
        free_ast(ast->for_condition);
        free_ast(ast->for_increment);
        free_ast(ast->for_body);
        break;
    case AST_RETURN_STATEMENT:
        free_ast(ast->return_value);
        break;
    case AST_BLOCK:
        for (int i = 0; i < ast->block_statement_count; i++)
        {
            free_ast(ast->block_statements[i]);
        }
        free(ast->block_statements);
        break;
    case AST_EXPRESSION_STATEMENT:
        free_ast(ast->expression_statement);
        break;
    }

    free(ast);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(1);
    }

    char *input_file = argv[1];
    FILE *file = fopen(input_file, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open input file: %s\n", input_file);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *input = (char *)malloc((file_size + 1) * sizeof(char));
    fread(input, sizeof(char), file_size, file);
    input[file_size] = '\0';
    fclose(file);

    Token *tokens = lexer(input);
    AST *ast = parse(tokens);
    semantic_analysis(ast);
    interpret(ast);

    free(input);
    free(tokens);

    // Free the AST
    free_ast(ast);

    // Free the symbol table
    for (int i = 0; i < symbol_count; i++)
    {
        free(symbol_table[i].name);
    }
    free(symbol_table);

    // Free the variable table
    for (int i = 0; i < variable_count; i++)
    {
        free(variable_table[i].name);
    }
    free(variable_table);

    return 0;
}