#ifndef MAIN_H
#define MAIN_H

// Symbol table entry
typedef struct
{
    char *name;
    int type;
} SymbolEntry;

// Symbol table
extern SymbolEntry *symbol_table;
extern int symbol_count;

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
extern VariableEntry *variable_table;
extern int variable_count;

#endif