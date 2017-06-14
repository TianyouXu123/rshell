#!/bin/sh

g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "pipe_test:"
str="cat < test.txt | tr A-Z a-z | tee output2.txt | tr a-z A-Z > output3.txt && exit"

echo ${str}
{
    echo ${str}
    sleep 1
} | ./test

rm test
