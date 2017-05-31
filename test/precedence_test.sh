#!/bin/sh

g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "precedence_test:"
str="echo A && echo B || echo C && echo D && echo A && echo B || (echo C && echo D || echo E); exit"
echo ${str}
{
	echo ${str}
	sleep 1
} | ./test

rm test
