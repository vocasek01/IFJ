PROJ=prg.code
FILES=main.c
CFLAGS=-std=gnu99 -Wextra -Werror -pedantic -pthread
CC=gcc
RM=rm -f

all : $(PROJ)

$(PROJ) : $(FILES)
		$(CC) $(CFLAGS) -o $(PROJ) $(FILES) -lm
