PROJ=proj
FILES=scanner.c expression.c parser.c stack.c symtable.c
CFLAGS=-std=gnu99 -Wextra -Werror -pedantic -pthread
CC=gcc
RM=rm -f

all : $(PROJ)

$(PROJ) : $(FILES)
		$(CC) $(CFLAGS) -o $(PROJ) $(FILES) -lm

test :
	./tests/tests_runner.sh

clean :
	$(RM) *.o $(PROJ)