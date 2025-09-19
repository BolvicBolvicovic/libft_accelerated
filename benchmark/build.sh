#!/bin/bash

CC=gcc
EXEC=benchmark.out
FILES=benchmark.c

${CC} ${FILES} -I. -O2 -march=native -o ${EXEC} 
