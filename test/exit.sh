#!/bin/sh
#tests commands with ;, &&, or ||

g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "command with exit test:"
str="ls -a; echo world && touch world || echo fail; ls && exit"

echo ${str}
{
        echo ${str}
	sleep 1
} | ./test

rm world
rm test
