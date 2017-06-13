#!/bin/sh

g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "output_redirect_test:"
str="cat < src/main.c | tr a-z A-Z > output1.txt && exit"

echo ${str}
{
  echo ${str}
	sleep 1
} | ./test

rm test
