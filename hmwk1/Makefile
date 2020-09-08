# Dalio, Brian A.
# dalioba
# 2020-09-07

FILES  = lex.yy.c parser.tab.c expr.c main.c
CC     = gcc
CFLAGS = -g -Wall -Wextra -fsanitize=address -fsanitize=leak -static-libasan

calc : $(FILES)
	@echo "\n#-- Put it all together ----------------"
	$(CC) $(CFLAGS) $(FILES) -o calc -lreadline

lex.yy.c : lexer.l
	@echo "\n#-- Lexical Analyzer -------------------"
	flex lexer.l

parser.tab.c : parser.y lex.yy.c
	@echo "\n#-- Syntactic Analyzer -----------------"
	bison --report=all --xml parser.y
	xsltproc /usr/share/bison/xslt/xml2xhtml.xsl parser.xml > parser.html
	rm -f parser.xml

clean:
	rm -f *.o lex.yy.c lex.yy.h parser.tab.c parser.tab.h parser.output parser.xml parser.html calc
