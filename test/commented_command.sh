#!/bin/sh
#tests commands with comments

g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "test"
str="echo hello world # this is a comment; exit"

echo ${str}
{
	echo ${str}
	sleep 1
} | ./test

rm test
