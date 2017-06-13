#!/bin/sh

g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "append_redirect_test:"
str="cat < test/test.txt | tr A-Z a-z | tee test/output4.txt | tr a-z A-Z >> test/output5.txt && exit"

echo ${str}
{
  echo ${str}
  sleep 1
} | ./test

rm test
