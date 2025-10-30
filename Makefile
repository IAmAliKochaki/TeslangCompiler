CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC = main.c lexer/lexer.c lexer/token.c
OUT = lex

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
