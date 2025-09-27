#!/bin/bash

CC=gcc
CFLAGS="-g -march=native -Wall -Wextra -Werror"
EXEC=tester.out
FILES=tests.c
LIB="-L.. -lft"

$CC $CFLAGS -I.. $FILES $LIB -o $EXEC
