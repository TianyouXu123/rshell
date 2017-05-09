#!/bin/sh
#tests single commands
g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "----------"
echo "echo This is a test"
exit

rm test
