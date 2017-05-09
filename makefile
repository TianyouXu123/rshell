all: rshell

rshell: src/main.c
	mkdir bin
	g++ -Wall -Werror -ansi -pedantic src/main.c -o bin/rshell
