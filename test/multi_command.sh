#!/bin/sh
#tests commands with ;, &&, or ||

g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "test 1"
str="ls -a; echo hello && rm hello || echo world; exit"

echo ${str}
{
	echo ${str}
	sleep 1
} | ./test

echo "test 2"
str="ls -a; echo world && touch world || echo sucsess; ls &&  exit"

echo ${str}
{
        echo ${str}
	sleep 1
} | ./test

rm world
rm test
