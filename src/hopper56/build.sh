#!/usr/bin/env sh
SRC_PATH=.
CC=g++
LD=g++
CFLAGS=" -DDEBUG -std=c++11 -g -Wall"
LDFLAGS="-lc"

# Just build everything -- this project isn't big
# lib code

# Application code
${CC} ${CFLAGS} -c -o decode.o      decode.cpp
${CC} ${CFLAGS} -c -o instruction.o instruction.cpp
${CC} ${CFLAGS} -c -o main.o        main.cpp

${LD} ${LDFLAGS} main.o decode.o instruction.o -o test


