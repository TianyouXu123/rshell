#!/bin/sh

g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "pipe_test:"
str="$ cat < src/main.c | tr A-Z a-z | tee output1.txt | tr a-z A-Z > output2.txt && exit"

echo ${str}
{
  echo ${str}
	sleep 1
} | ./test

rm test
