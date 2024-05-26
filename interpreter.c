#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "ast.h"
#include "main.h"

// Function to add a variable to the variable table
void add_variable(char *name, int type, void *value)
{
    variable_table = (VariableEntry *)realloc(variable_table, (variable_count + 1) * sizeof(VariableEntry));
    variable_table[variable_count].name = strdup(name);
    variable_table[variable_count].type = type;
    if (type == AST_INTEGER)
    {
        variable_table[variable_count].integer_value = *(int *)value;
    }
    else if (type == AST_FLOAT)
    {
        variable_table[variable_count].float_value = *(float *)value;
    }
    variable_count++;
}

// Function to look up a variable in the variable table
VariableEntry *lookup_variable(char *name)
{
    for (int i = 0; i < variable_count; i++)
    {
        if (strcmp(variable_table[i].name, name) == 0)
        {
            return &variable_table[i];
        }
    }
    return NULL;
}

// Function to interpret a function call
void interpret_function_call(AST *ast)
{
    // Perform function call logic here
    // You can define a separate function table to store function definitions
    // and look up the function by its name to execute it
    // For simplicity, let's assume the function just prints its name and arguments
    printf("Function call: %s\n", ast->function_call_name);
    printf("Arguments:\n");
    for (int i = 0; i < ast->function_call_argument_count; i++)
    {
        interpret(&ast->function_call_arguments[i]);
        VariableEntry *entry = lookup_variable(ast->function_call_arguments[i].identifier_name);
        if (entry == NULL)
        {
            fprintf(stderr, "Undefined variable in function argument\n");
            exit(1);
        }
        if (entry->type == AST_INTEGER)
        {
            printf("%d\n", entry->integer_value);
        }
        else if (entry->type == AST_FLOAT)
        {
            printf("%f\n", entry->float_value);
        }
    }
}

// Function to interpret a return statement
void interpret_return_statement(AST *ast)
{
    interpret(ast->return_value);
    // Perform return logic here
    // You can store the return value in a separate variable or pass it up the call stack
    // For simplicity, let's just print the return value
    VariableEntry *entry = lookup_variable(ast->return_value->identifier_name);
    if (entry == NULL)
    {
        fprintf(stderr, "Undefined variable in return statement\n");
        exit(1);
    }
    if (entry->type == AST_INTEGER)
    {
        printf("Return value: %d\n", entry->integer_value);
    }
    else if (entry->type == AST_FLOAT)
    {
        printf("Return value: %f\n", entry->float_value);
    }
}

// Function to interpret a binary expression
void interpret_binary_expression(AST *ast)
{
    interpret(ast->binary_left);
    interpret(ast->binary_right);

    VariableEntry *left = lookup_variable(ast->binary_left->identifier_name);
    VariableEntry *right = lookup_variable(ast->binary_right->identifier_name);

    if (left == NULL || right == NULL)
    {
        fprintf(stderr, "Undefined variable in binary expression\n");
        exit(1);
    }

    if (left->type != right->type)
    {
        fprintf(stderr, "Type mismatch in binary expression\n");
        exit(1);
    }

    switch (ast->binary_operator)
    {
    case TOKEN_PLUS:
        if (left->type == AST_INTEGER)
        {
            int result = left->integer_value + right->integer_value;
            add_variable(ast->identifier_name, AST_INTEGER, &result);
        }
        else if (left->type == AST_FLOAT)
        {
            float result = left->float_value + right->float_value;
            add_variable(ast->identifier_name, AST_FLOAT, &result);
        }
        break;
    case TOKEN_MINUS:
        if (left->type == AST_INTEGER)
        {
            int result = left->integer_value - right->integer_value;
            add_variable(ast->identifier_name, AST_INTEGER, &result);
        }
        else if (left->type == AST_FLOAT)
        {
            float result = left->float_value - right->float_value;
            add_variable(ast->identifier_name, AST_FLOAT, &result);
        }
        break;
    case TOKEN_MULTIPLY:
        if (left->type == AST_INTEGER)
        {
            int result = left->integer_value * right->integer_value;
            add_variable(ast->identifier_name, AST_INTEGER, &result);
        }
        else if (left->type == AST_FLOAT)
        {
            float result = left->float_value * right->float_value;
            add_variable(ast->identifier_name, AST_FLOAT, &result);
        }
        break;
    case TOKEN_DIVIDE:
        if (right->integer_value == 0 || right->float_value == 0.0)
        {
            fprintf(stderr, "Division by zero\n");
            exit(1);
        }
        if (left->type == AST_INTEGER)
        {
            int result = left->integer_value / right->integer_value;
            add_variable(ast->identifier_name, AST_INTEGER, &result);
        }
        else if (left->type == AST_FLOAT)
        {
            float result = left->float_value / right->float_value;
            add_variable(ast->identifier_name, AST_FLOAT, &result);
        }
        break;
    default:
        fprintf(stderr, "Unknown binary operator: %d\n", ast->binary_operator);
        exit(1);
    }
}

// Function to interpret an AST node
void interpret(AST *ast)
{
    if (ast == NULL)
    {
        return;
    }

    switch (ast->type)
    {
    case AST_VARIABLE_DECLARATION:
    {
        interpret(ast->variable_declaration_value);
        VariableEntry *entry = lookup_variable(ast->variable_declaration_value->identifier_name);
        if (entry == NULL)
        {
            fprintf(stderr, "Undefined variable in variable declaration\n");
            exit(1);
        }
        add_variable(ast->variable_declaration_name, entry->type, &entry->integer_value);
        break;
    }
    case AST_BINARY_EXPRESSION:
    {
        interpret_binary_expression(ast);
        break;
    }
    case AST_INTEGER:
    {
        add_variable(ast->identifier_name, AST_INTEGER, &ast->integer_value);
        break;
    }
    case AST_FLOAT:
    {
        add_variable(ast->identifier_name, AST_FLOAT, &ast->float_value);
        break;
    }
    case AST_IDENTIFIER:
    {
        VariableEntry *entry = lookup_variable(ast->identifier_name);
        if (entry == NULL)
        {
            fprintf(stderr, "Undefined variable: %s\n", ast->identifier_name);
            exit(1);
        }
        break;
    }
    case AST_FUNCTION_CALL:
    {
        interpret_function_call(ast);
        break;
    }
    case AST_IF_STATEMENT:
    {
        interpret(ast->if_condition);
        VariableEntry *entry = lookup_variable(ast->if_condition->identifier_name);
        if (entry == NULL)
        {
            fprintf(stderr, "Undefined variable in if condition\n");
            exit(1);
        }
        if (entry->integer_value)
        {
            interpret(ast->if_body);
        }
        else if (ast->if_else != NULL)
        {
            interpret(ast->if_else);
        }
        break;
    }
    case AST_WHILE_STATEMENT:
    {
        interpret(ast->while_condition);
        VariableEntry *entry = lookup_variable(ast->while_condition->identifier_name);
        if (entry == NULL)
        {
            fprintf(stderr, "Undefined variable in while condition\n");
            exit(1);
        }
        while (entry->integer_value)
        {
            interpret(ast->while_body);
            interpret(ast->while_condition);
            entry = lookup_variable(ast->while_condition->identifier_name);
        }
        break;
    }
    case AST_FOR_STATEMENT:
    {
        interpret(ast->for_init);
        interpret(ast->for_condition);
        VariableEntry *entry = lookup_variable(ast->for_condition->identifier_name);
        if (entry == NULL)
        {
            fprintf(stderr, "Undefined variable in for condition\n");
            exit(1);
        }
        while (entry->integer_value)
        {
            interpret(ast->for_body);
            interpret(ast->for_increment);
            interpret(ast->for_condition);
            entry = lookup_variable(ast->for_condition->identifier_name);
        }
        break;
    }
    case AST_RETURN_STATEMENT:
    {
        interpret_return_statement(ast);
        break;
    }
    case AST_BLOCK:
    {
        for (int i = 0; i < ast->block_statement_count; i++)
        {
            interpret(ast->block_statements[i]);
        }
        break;
    }
    case AST_EXPRESSION_STATEMENT:
    {
        interpret(ast->expression_statement);
        break;
    }
    default:
        fprintf(stderr, "Unknown AST node type: %d\n", ast->type);
        exit(1);
    }
}