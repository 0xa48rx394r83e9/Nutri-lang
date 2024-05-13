#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

// Function to create a new token
Token *create_token(TokenType type, char *value)
{
    Token *token = (Token *)malloc(sizeof(Token));
    token->type = type;
    token->value = strdup(value);
    return token;
}

// Lexer function to tokenize the input
Token *lexer(char *input)
{
    Token *tokens = NULL;
    int token_count = 0;

    while (*input)
    {
        // Skip whitespace
        while (isspace(*input))
        {
            input++;
        }

        if (*input == '\0')
        {
            break;
        }

        // Check for keywords and identifiers
        if (isalpha(*input) || *input == '_')
        {
            char *start = input;
            while (isalnum(*input) || *input == '_')
            {
                input++;
            }
            int length = input - start;
            char *value = (char *)malloc(length + 1);
            strncpy(value, start, length);
            value[length] = '\0';

            if (strcmp(value, "var") == 0)
            {
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_VAR, value);
            }
            else if (strcmp(value, "if") == 0)
            {
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_IF, value);
            }
            else if (strcmp(value, "else") == 0)
            {
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_ELSE, value);
            }
            else if (strcmp(value, "while") == 0)
            {
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_WHILE, value);
            }
            else if (strcmp(value, "for") == 0)
            {
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_FOR, value);
            }
            else if (strcmp(value, "func") == 0)
            {
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_FUNC, value);
            }
            else if (strcmp(value, "return") == 0)
            {
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_RETURN, value);
            }
            else
            {
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_IDENTIFIER, value);
            }
        }
        // Check for literals
        else if (isdigit(*input))
        {
            char *start = input;
            while (isdigit(*input))
            {
                input++;
            }
            if (*input == '.')
            {
                input++;
                while (isdigit(*input))
                {
                    input++;
                }
                int length = input - start;
                char *value = (char *)malloc(length + 1);
                strncpy(value, start, length);
                value[length] = '\0';
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_FLOAT, value);
            }
            else
            {
                int length = input - start;
                char *value = (char *)malloc(length + 1);
                strncpy(value, start, length);
                value[length] = '\0';
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_INT, value);
            }
        }
        // Check for operators and delimiters
        else
        {
            switch (*input)
            {
            case '+':
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_PLUS, "+");
                break;
            case '-':
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_MINUS, "-");
                break;
            case '*':
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_MULTIPLY, "*");
                break;
            case '/':
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_DIVIDE, "/");
                break;
            case '=':
                if (*(input + 1) == '=')
                {
                    tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                    tokens[token_count++] = *create_token(TOKEN_EQUAL, "==");
                    input++;
                }
                else
                {
                    tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                    tokens[token_count++] = *create_token(TOKEN_ASSIGN, "=");
                }
                break;
            case '!':
                if (*(input + 1) == '=')
                {
                    tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                    tokens[token_count++] = *create_token(TOKEN_NOT_EQUAL, "!=");
                    input++;
                }
                break;
            case '<':
                if (*(input + 1) == '=')
                {
                    tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                    tokens[token_count++] = *create_token(TOKEN_LESS_EQUAL, "<=");
                    input++;
                }
                else
                {
                    tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                    tokens[token_count++] = *create_token(TOKEN_LESS, "<");
                }
                break;
            case '>':
                if (*(input + 1) == '=')
                {
                    tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                    tokens[token_count++] = *create_token(TOKEN_GREATER_EQUAL, ">=");
                    input++;
                }
                else
                {
                    tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                    tokens[token_count++] = *create_token(TOKEN_GREATER, ">");
                }
                break;
            case '(':
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_LPAREN, "(");
                break;
            case ')':
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_RPAREN, ")");
                break;
            case '{':
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_LBRACE, "{");
                break;
            case '}':
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_RBRACE, "}");
                break;
            case ';':
                tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
                tokens[token_count++] = *create_token(TOKEN_SEMICOLON, ";");
                break;
            default:
                fprintf(stderr, "Invalid character: %c\n", *input);
                exit(1);
            }
            input++;
        }
    }

    tokens = (Token *)realloc(tokens, (token_count + 1) * sizeof(Token));
    tokens[token_count] = *create_token(TOKEN_EOF, "");

    return tokens;
}