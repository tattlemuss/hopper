#!/usr/bin/env sh
SRC_PATH=.
CC=g++
LD=g++
CFLAGS=" -DDEBUG -I${SRC_PATH}/lib -std=c++11 -g"
LDFLAGS="-lc"

# Just build everything -- this project isn't big
${CC} ${CFLAGS} -c -o symbols.o symbols.cpp
${CC} ${CFLAGS} -c -o decode.o decode.cpp
${CC} ${CFLAGS} -c -o instruction.o instruction.cpp
${CC} ${CFLAGS} -c -o timing.o timing.cpp
${CC} ${CFLAGS} -c -o main.o main.cpp

${LD} ${LDFLAGS} main.o instruction.o timing.o symbols.o decode.o -o bin/hopper


