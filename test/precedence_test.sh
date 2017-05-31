#!/bin/sh

g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "precedence_test:"
str1="echo A && echo B || echo C && echo D"
str2="echo A && echo B || (echo C && echo D || echo E)"
str3="exit"
echo ${str}
{
	echo ${str1}
	sleep 1
  echo ${str2}
  sleep 1
  echo ${str3}	
	sleep 1
} | ./test

rm test
