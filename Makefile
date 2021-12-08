PROJ=prg.code
FILES=main.c scanner.c parser.c expression.c stack.c symtable.c
CFLAGS=-std=gnu99 -Wextra -Werror -pedantic -pthread
CC=gcc
RM=rm -f

all : $(PROJ)

$(PROJ) : $(FILES)
		$(CC) $(CFLAGS) -o $(PROJ) $(FILES) -lm
