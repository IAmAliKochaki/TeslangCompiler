#ifndef TOKEN_H
#define TOKEN_H

#define MAX_TOKEN_LEN 256

typedef enum {
        // Keywords
        FN, AS, RETURN, IF, ELSE, DO, WHILE, FOR, TO, BEGIN, END, LEN,
        INT, VECTOR, STR, BOOLEAN, TRUE, FALSE, NILL,
        
        // Identifiers and literals
        ID, NUMBER, STRING,
        
        // Operators
        PLUS, MINUS, MULT, DIV,
        EQ, DBL_COLON,
        LESS_THAN, GREATER_THAN, LE, GE, EQEQ, NEQ,
        AND, OR, NOT,
        
        // Symbols
        LPAREN, RPAREN, LCURLYEBR, RCURLYEBR, LSQUAREBR, RSQUAREBR,
        SEMI_COLON, COMMA,

        // Null pointer
        NULLP
} TokenType;

extern const char *type_name[];

typedef struct {
    int line;
    int column;
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

typedef struct
{
    char *kw;
    TokenType type;
}Keyword;

extern const Keyword keywords[];

#endif // TOKEN_H
