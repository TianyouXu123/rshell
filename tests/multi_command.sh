#!/bin/sh
g++ -Wall -Werror -ansi -pedantic ../src/main.c -o test

echo "----------"
str="ls -a; echo hello && rm hello || echo world; exit"

echo ${str}
{
	echo ${str}
	sleep 1
} | ./test

echo "----------"
str="ls -a; echo world && touch world || echo sucsess; ls &&  exit"

echo ${str}
{
  echo ${str}
	sleep 1
} | ./test

rm hello test
