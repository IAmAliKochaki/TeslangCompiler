#include "token.h"

const Keyword keywords[] = {
    {"fn", FN}, {"as", AS}, {"return", RETURN}, {"if", IF}, {"else", ELSE},
    {"do", DO}, {"while", WHILE}, {"for", FOR}, {"to", TO}, {"begin", BEGIN}, {"end", END},
    {"int", INT}, {"vector", VECTOR}, {"str", STR}, {"boolean", BOOLEAN},
    {"true", TRUE}, {"false", FALSE}, {"null", NILL},
    {((void *)0), NULLP}
};

const char *type_name[] = {

        // Keywords
        "FN", "AS", "RETURN", "IF", "ELSE", "DO", "WHILE", "FOR", "TO", "BEGIN", "END", "LEN",
        "INT", "VECTOR", "STR", "BOOLEAN", "TRUE", "FALSE", "NILL",
        
        // Identifiers and literals
        "ID", "NUMBER", "STRING",
        
        // Operators
        "PLUS", "MINUS", "MULT", "DIV",
        "EQ", "DBL_COLON",
        "LESS_THAN", "GREATER_THAN", "LE", "GE", "EQEQ", "NEQ",
        "AND", "OR", "NOT",
        
        // Symbols
        "LPAREN", "RPAREN", "LCURLYEBR", "RCURLYEBR", "LSQUAREBR", "RSQUAREBR",
        "SEMI_COLON", "COMMA",

        // Null pointer
        "NULLP"
};