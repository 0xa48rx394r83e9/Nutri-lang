#ifndef AST_H
#define AST_H

#include "lexer.h"

typedef enum
{
    AST_BLOCK,
    AST_VARIABLE_DECLARATION,
    AST_IF_STATEMENT,
    AST_WHILE_STATEMENT,
    AST_FOR_STATEMENT,
    AST_RETURN_STATEMENT,
    AST_EXPRESSION_STATEMENT,
    AST_BINARY_EXPRESSION,
    AST_FUNCTION_CALL,
    AST_IDENTIFIER,
    AST_INTEGER,
    AST_FLOAT
} ASTNodeType;

typedef struct ASTNode
{
    ASTNodeType type;
    union
    {
        // Block
        struct
        {
            struct ASTNode **block_statements;
            int block_statement_count;
        };
        // Variable Declaration
        struct
        {
            char *variable_declaration_name;
            struct ASTNode *variable_declaration_value;
        };
        // If Statement
        struct
        {
            struct ASTNode *if_condition;
            struct ASTNode *if_body;
            struct ASTNode *if_else;
        };
        // While Statement
        struct
        {
            struct ASTNode *while_condition;
            struct ASTNode *while_body;
        };
        // For Statement
        struct
        {
            struct ASTNode *for_init;
            struct ASTNode *for_condition;
            struct ASTNode *for_increment;
            struct ASTNode *for_body;
        };
        // Return Statement
        struct
        {
            struct ASTNode *return_value;
        };
        // Expression Statement
        struct
        {
            struct ASTNode *expression_statement;
        };
        // Binary Expression
        struct
        {
            TokenType binary_operator;
            struct ASTNode *binary_left;
            struct ASTNode *binary_right;
        };
        // Function Call
        struct
        {
            char *function_call_name;
            struct ASTNode *function_call_arguments;
            int function_call_argument_count;
        };
        // Identifier
        struct
        {
            char *identifier_name;
        };
        // Integer
        struct
        {
            int integer_value;
        };
        // Float
        struct
        {
            float float_value;
        };
    };
} AST;

AST *create_ast_node(ASTNodeType type);

#endif