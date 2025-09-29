#!/bin/bash

CC=gcc
LIB=libft.a
CFILES="./src/*.c"
OFILES="*.o"
FLAGS="-c -O2 -g -march=native -Wall -Wextra -Werror"

$CC -I. $FLAGS $CFILES
ar rcs	$LIB $OFILES
