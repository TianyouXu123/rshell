#!/bin/sh
#tests single commands

g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "----------"
str1="echo hello"
str2="exit"
echo ${str}
{
	echo ${str1}
	sleep 1
        echo ${str2}
	sleep 1
} | ./test

rm test
