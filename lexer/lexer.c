#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

static int initialized = 0;
static char *src;
static int pos;
static int line;
static int column;

static void init_lexer(char *);
static char peek_char();
static Token id_kw(Token);
static Token num(Token);
static Token str(Token);
static void opr_sym(Token *);
static void skip_ws_comments();
static void _exit(int);

Token get_token(char *curr_head)
{
    init_lexer(curr_head);

    skip_ws_comments();

    Token tok;
    tok.line = line;
    tok.column = column;
    tok.value[0] = '\0';

    char c = peek_char();

    /* Check for end of file */
    if (c == '\0')
    {
        tok.type = NULLP;
        strcpy(tok.value, "EOF");
        return tok;
    }

    /* Identifier or Keyword*/
    if (isalpha(c) || c == '_')
        return id_kw(tok);

    /* Number */
    if (isdigit(c))
        return num(tok);

    /* String */
    if (c == '"' || c == '\'')
        return str(tok);

    /* Operators and Symbols */
    opr_sym(&tok);

    return tok;
}

static void init_lexer(char *curr_head)
{
    if (!initialized)
    {
        src = curr_head;
        pos = 0;
        line = 1;
        column = 1;
        initialized = 1;
    }
}

static char peek_char()
{
    return src[pos];
}

static char next_char()
{
    char c = src[pos++];
    if (c == '\n')
    {
        line++;
        column = 1;
    }
    else if (c == '\t')
        column += 4;
    else
        column++;
    return c;
}

static TokenType check_keyword(const char *word)
{
    for (int i = 0; keywords[i].kw != NULL; i++)
    {
        if (strcmp(word, keywords[i].kw) == 0)
            return keywords[i].type;
    }
    return ID;
}

static void comment()
{
    int comment_level = 1;
    while (comment_level > 0)
    {
        if (peek_char() == '\0')
        {
            fprintf(stderr, "error\n");
            _exit(1);
        }

        if (peek_char() == '<')
        {
            next_char();
            if (peek_char() == '%')
                comment_level++;
        }
        else if (peek_char() == '%')
        {
            next_char();
            if (peek_char() == '>')
                comment_level--;
        }
        next_char();
    }
}

static void throw_error(Token token)
{
    fprintf(stderr, "%s\n", token.value);
    fprintf(stderr, "line %d, column %d\n", token.line, token.column);
}

static void unknown_token(Token tok, char c)
{
    tok.type = NULLP;
    sprintf(tok.value, "Unknown(%c)", c);
    throw_error(tok);
    _exit(1);
}

static Token id_kw(Token tok)
{
    int i = 0;
    while (isalnum(peek_char()) || peek_char() == '_')
        tok.value[i++] = next_char();
    tok.value[i] = '\0';
    tok.type = check_keyword(tok.value);
    return tok;
}

static Token num(Token tok)
{
    int i = 0;
    while (isdigit(peek_char()))
        tok.value[i++] = next_char();
    tok.value[i] = '\0';
    tok.type = NUMBER;
    return tok;
}

static Token str(Token tok)
{
    char quote = next_char();
    int i = 0;
    while (peek_char() != quote)
    {
        if (peek_char() == '\0')
        {
            fprintf(stderr, "Error, define string\n");
            _exit(1);
        }

        if (peek_char() == '\\')
        {
            tok.value[i++] = next_char();
            char esc = peek_char();
            switch (esc)
            {
            case 'n':
                tok.value[i++] = 'n';
                break;
            case 't':
                tok.value[i++] = 't';
                break;
            case 'r':
                tok.value[i++] = 'r';
                break;
            case '\\':
                tok.value[i++] = '\\';
                break;
            case '"':
                tok.value[i++] = '\"';
                break;
            case '\'':
                tok.value[i++] = '\'';
                break;
            case '0':
                tok.value[i++] = '\0';
                break;
            default:
                // Unknown escape sequence, copy as-is
                tok.value[i++] = esc;
                break;
            }
            next_char();
            continue;
        }
        tok.value[i++] = next_char();
    }
    tok.value[i] = '\0';
    if (peek_char() == quote)
        next_char();
    tok.type = STRING;
    return tok;
}

static void opr_sym(Token *tok)
{
    char c = next_char();
    switch (c)
    {
    case '+':
        tok->type = PLUS;
        strcpy(tok->value, "+");
        break;
    case '-':
        tok->type = MINUS;
        strcpy(tok->value, "-");
        break;
    case '*':
        tok->type = MULT;
        strcpy(tok->value, "*");
        break;
    case '/':
        tok->type = DIV;
        strcpy(tok->value, "/");
        break;
    case '=':
        if (peek_char() == '=')
        {
            next_char();
            tok->type = EQEQ;
            strcpy(tok->value, "==");
        }
        else
        {
            tok->type = EQ;
            strcpy(tok->value, "=");
        }
        break;
    case ':':
        if (peek_char() == ':')
        {
            next_char();
            tok->type = DBL_COLON;
            strcpy(tok->value, "::");
        }
        else
            unknown_token(*tok, c);
        break;
    case '<':
        tok->type = LESS_THAN;
        strcpy(tok->value, "<");
        break;
    case '>':
        tok->type = GREATER_THAN;
        strcpy(tok->value, ">");
        break;
    case '(':
        tok->type = LPAREN;
        strcpy(tok->value, "(");
        break;
    case ')':
        tok->type = RPAREN;
        strcpy(tok->value, ")");
        break;
    case '{':
        tok->type = LCURLYEBR;
        strcpy(tok->value, "{");
        break;
    case '}':
        tok->type = RCURLYEBR;
        strcpy(tok->value, "}");
        break;
    case '[':
        tok->type = LSQUAREBR;
        strcpy(tok->value, "[");
        break;
    case ']':
        tok->type = RSQUAREBR;
        strcpy(tok->value, "]");
        break;
    case ';':
        tok->type = SEMI_COLON;
        strcpy(tok->value, ";");
        break;
    case ',':
        tok->type = COMMA;
        strcpy(tok->value, ",");
        break;
    default:
        unknown_token(*tok, c);
    }
}

static void skip_ws_comments()
{
    do
    {
        while (isspace(peek_char()))
            next_char();

        /* Comment */
        if (peek_char() == '<' && src[pos + 1] == '%')
        {
            next_char();
            next_char();
            comment();
        }
    } while (isspace(peek_char()) || (peek_char() == '<' && src[pos + 1] == '%'));
}

static void _exit(int r)
{
    free(src);
    exit(1);
}