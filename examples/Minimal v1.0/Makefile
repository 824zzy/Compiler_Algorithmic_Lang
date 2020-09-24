# Dalio, Brian A.
# dalioba
# 2020-09-17

PROJS  = minimal1 minimal2
CC     = gcc
CFLAGS = -g -Wall -Wextra -fsanitize=address -fsanitize=leak -static-libasan

all : $(PROJS)

minimal1 : minimal1.l
	flex minimal1.l
	$(CC) $(CFLAGS) -o minimal1 lex.yy.c

minimal2 : minimal2.l
	flex minimal2.l
	$(CC) $(CFLAGS) -o minimal2 lex.yy.c

clean:
	rm -f *.o $(PROJS) lex.yy.[ch] minimal?.tab.[ch]
