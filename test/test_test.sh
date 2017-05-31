#!/bin/sh
#tests single commands

g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "test command test:"

str="test -e test/test_test.sh && echo "File found!"
echo ${str}
{
	echo ${str}
	sleep 1
} | ./test

str="[ -e test/123.txt ] || echo "File not found!"
echo ${str}
{
	echo ${str}
	sleep 1
} | ./test

str="exit"
echo ${str}
{
	echo ${str}
	sleep 1
} | ./test

rm test
