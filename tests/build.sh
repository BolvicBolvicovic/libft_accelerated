#!/bin/bash

CC=gcc
CFLAGS="-g -march=native -Wall -Wextra -Werror"
EXEC=tester.out
FILES=tests.c
LIB="-L.. -lft"

cd .. && rm libft.a *.o; ./build.sh && cd tests && $CC $CFLAGS -I.. $FILES $LIB -o $EXEC
