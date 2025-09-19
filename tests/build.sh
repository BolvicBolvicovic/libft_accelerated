#!/bin/bash

CC=gcc
EXEC=tester.out
FILES=tests.c

${CC} -I. ${FILES} -g -march=native -o ${EXEC} 
