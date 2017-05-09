#!/bin/sh
#tests single commands
g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "----------"
str="echo unhappy # this command will print out unhappy; exit # quit"

echo ${str}
{
	echo ${str}
	sleep 1
} | ./test

rm a.out
