#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

AST *parse(Token *tokens);

#endif