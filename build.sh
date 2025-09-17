#!/bin/bash

CC=gcc
EXEC=tester
FILES=tests.c

${CC} -I. ${FILES} -g -msse2 -mbmi -o ${EXEC} 
