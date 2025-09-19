#!/bin/bash

CC=gcc
EXEC=benchmark_old.out
FILES=benchmark_old.c
INC=./libft_old
LIB=libft.a

if [ ! -f $INC/$LIB ]; then
	make -C $INC
fi

$CC $FILES -I$INC -L$INC -l:$LIB -O2 -march=native -o $EXEC 
