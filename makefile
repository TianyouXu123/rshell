all: rshell

rshell: src/main.c
	mkdir bin
	g++ -Wall -Werror -ansi -pedantic src/main.cpp -o bin/rshell
