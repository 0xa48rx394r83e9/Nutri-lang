#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include "ast.h"

// Function prototypes
AST *parse_expression(Token **tokens);
AST *parse_primary(Token **tokens);
AST *parse_statement(Token **tokens);
AST *parse_block(Token **tokens);

// Parser function to construct the AST
AST *parse(Token *tokens)
{
    AST *ast = parse_block(&tokens);
    if (tokens->type != TOKEN_EOF)
    {
        fprintf(stderr, "Unexpected token: %s\n", tokens->value);
        exit(1);
    }
    return ast;
}

// Parse a block of statements
AST *parse_block(Token **tokens)
{
    AST *block = create_ast_node(AST_BLOCK);
    block->block_statements = NULL;
    block->block_statement_count = 0;

    while ((*tokens)->type != TOKEN_RBRACE && (*tokens)->type != TOKEN_EOF)
    {
        AST *statement = parse_statement(tokens);
        block->block_statements = (AST **)realloc(block->block_statements, (block->block_statement_count + 1) * sizeof(AST *));
        block->block_statements[block->block_statement_count++] = statement;
    }

    if ((*tokens)->type == TOKEN_RBRACE)
    {
        (*tokens)++;
    }

    return block;
}

// Parse a statement
AST *parse_statement(Token **tokens)
{
    AST *statement = NULL;

    switch ((*tokens)->type)
    {
    case TOKEN_VAR:
    {
        (*tokens)++;
        if ((*tokens)->type != TOKEN_IDENTIFIER)
        {
            fprintf(stderr, "Expected identifier after 'var'\n");
            exit(1);
        }
        char *var_name = strdup((*tokens)->value);
        (*tokens)++;
        if ((*tokens)->type != TOKEN_ASSIGN)
        {
            fprintf(stderr, "Expected '=' after variable name\n");
            exit(1);
        }
        (*tokens)++;
        AST *expression = parse_expression(tokens);
        if ((*tokens)->type != TOKEN_SEMICOLON)
        {
            fprintf(stderr, "Expected ';' after variable declaration\n");
            exit(1);
        }
        (*tokens)++;
        statement = create_ast_node(AST_VARIABLE_DECLARATION);
        statement->variable_declaration_name = var_name;
        statement->variable_declaration_value = expression;
        break;
    }
    case TOKEN_IF:
    {
        (*tokens)++;
        if ((*tokens)->type != TOKEN_LPAREN)
        {
            fprintf(stderr, "Expected '(' after 'if'\n");
            exit(1);
        }
        (*tokens)++;
        AST *condition = parse_expression(tokens);
        if ((*tokens)->type != TOKEN_RPAREN)
        {
            fprintf(stderr, "Expected ')' after if condition\n");
            exit(1);
        }
        (*tokens)++;
        AST *if_body = parse_block(tokens);
        AST *else_body = NULL;
        if ((*tokens)->type == TOKEN_ELSE)
        {
            (*tokens)++;
            else_body = parse_block(tokens);
        }
        statement = create_ast_node(AST_IF_STATEMENT);
        statement->if_condition = condition;
        statement->if_body = if_body;
        statement->if_else = else_body;
        break;
    }
    case TOKEN_WHILE:
    {
        (*tokens)++;
        if ((*tokens)->type != TOKEN_LPAREN)
        {
            fprintf(stderr, "Expected '(' after 'while'\n");
            exit(1);
        }
        (*tokens)++;
        AST *condition = parse_expression(tokens);
        if ((*tokens)->type != TOKEN_RPAREN)
        {
            fprintf(stderr, "Expected ')' after while condition\n");
            exit(1);
        }
        (*tokens)++;
        AST *while_body = parse_block(tokens);
        statement = create_ast_node(AST_WHILE_STATEMENT);
        statement->while_condition = condition;
        statement->while_body = while_body;
        break;
    }
    case TOKEN_FOR:
    {
        (*tokens)++;
        if ((*tokens)->type != TOKEN_LPAREN)
        {
            fprintf(stderr, "Expected '(' after 'for'\n");
            exit(1);
        }
        (*tokens)++;
        AST *init = parse_statement(tokens);
        AST *condition = parse_expression(tokens);
        if ((*tokens)->type != TOKEN_SEMICOLON)
        {
            fprintf(stderr, "Expected ';' after for condition\n");
            exit(1);
        }
        (*tokens)++;
        AST *increment = parse_expression(tokens);
        if ((*tokens)->type != TOKEN_RPAREN)
        {
            fprintf(stderr, "Expected ')' after for increment\n");
            exit(1);
        }
        (*tokens)++;
        AST *for_body = parse_block(tokens);
        statement = create_ast_node(AST_FOR_STATEMENT);
        statement->for_init = init;
        statement->for_condition = condition;
        statement->for_increment = increment;
        statement->for_body = for_body;
        break;
    }
    case TOKEN_RETURN:
    {
        (*tokens)++;
        AST *expression = parse_expression(tokens);
        if ((*tokens)->type != TOKEN_SEMICOLON)
        {
            fprintf(stderr, "Expected ';' after return statement\n");
            exit(1);
        }
        (*tokens)++;
        statement = create_ast_node(AST_RETURN_STATEMENT);
        statement->return_value = expression;
        break;
    }
    default:
    {
        AST *expression = parse_expression(tokens);
        if ((*tokens)->type != TOKEN_SEMICOLON)
        {
            fprintf(stderr, "Expected ';' after expression statement\n");
            exit(1);
        }
        (*tokens)++;
        statement = create_ast_node(AST_EXPRESSION_STATEMENT);
        statement->expression_statement = expression;
        break;
    }
    }

    return statement;
}

// Parse an expression
AST *parse_expression(Token **tokens)
{
    AST *left = parse_primary(tokens);

    while ((*tokens)->type == TOKEN_PLUS || (*tokens)->type == TOKEN_MINUS || (*tokens)->type == TOKEN_MULTIPLY || (*tokens)->type == TOKEN_DIVIDE)
    {
        Token *op = *tokens;
        (*tokens)++;
        AST *right = parse_primary(tokens);
        AST *binary_op = create_ast_node(AST_BINARY_EXPRESSION);
        binary_op->binary_operator = op->type;
        binary_op->binary_left = left;
        binary_op->binary_right = right;
        left = binary_op;
    }

    return left;
}

// Parse a primary expression
AST *parse_primary(Token **tokens)
{
    AST *primary = NULL;

    switch ((*tokens)->type)
    {
    case TOKEN_INT:
    {
        primary = create_ast_node(AST_INTEGER);
        primary->integer_value = atoi((*tokens)->value);
        (*tokens)++;
        break;
    }
    case TOKEN_FLOAT:
    {
        primary = create_ast_node(AST_FLOAT);
        primary->float_value = atof((*tokens)->value);
        (*tokens)++;
        break;
    }
    case TOKEN_IDENTIFIER:
    {
        char *identifier = strdup((*tokens)->value);
        (*tokens)++;
        if ((*tokens)->type == TOKEN_LPAREN)
        {
            (*tokens)++;
            AST *args = NULL;
            int arg_count = 0;
            if ((*tokens)->type != TOKEN_RPAREN)
            {
                args = parse_expression(tokens);
                arg_count++;
                while ((*tokens)->type == TOKEN_COMMA)
                {
                    (*tokens)++;
                    AST *arg = parse_expression(tokens);
                    args = (AST *)realloc(args, (arg_count + 1) * sizeof(AST));
                    args[arg_count++] = *arg;
                }
            }
            if ((*tokens)->type != TOKEN_RPAREN)
            {
                fprintf(stderr, "Expected ')' after function arguments\n");
                exit(1);
            }
            (*tokens)++;
            primary = create_ast_node(AST_FUNCTION_CALL);
            primary->function_call_name = identifier;
            primary->function_call_arguments = args;
            primary->function_call_argument_count = arg_count;
        }
        else
        {
            primary = create_ast_node(AST_IDENTIFIER);
            primary->identifier_name = identifier;
        }
        break;
    }
    case TOKEN_LPAREN:
    {
        (*tokens)++;
        primary = parse_expression(tokens);
        if ((*tokens)->type != TOKEN_RPAREN)
        {
            fprintf(stderr, "Expected ')' after expression\n");
            exit(1);
        }
        (*tokens)++;
        break;
    }
    default:
    {
        fprintf(stderr, "Unexpected token: %s\n", (*tokens)->value);
        exit(1);
    }
    }

    return primary;
}