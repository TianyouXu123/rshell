#!/bin/sh

g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "input_redirect_test:"
str="cat < test/test.txt && exit"

echo ${str}
{
        echo ${str}
	sleep 1
} | ./test

rm test
