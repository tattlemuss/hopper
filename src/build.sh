#!/usr/bin/env sh
SRC_PATH=.
CC=g++
LD=g++
CFLAGS=" -DDEBUG -std=c++11 -g"
LDFLAGS="-lc"

# Just build everything -- this project isn't big
# lib code
${CC} ${CFLAGS} -c -o decode.o      hopper68/decode.cpp
${CC} ${CFLAGS} -c -o instruction.o hopper68/instruction.cpp
${CC} ${CFLAGS} -c -o timing.o      hopper68/timing.cpp

# Application code
${CC} ${CFLAGS} -c -o symbols.o     symbols.cpp
${CC} ${CFLAGS} -c -o print.o       print.cpp
${CC} ${CFLAGS} -c -o main.o        main.cpp

${LD} ${LDFLAGS} main.o print.o instruction.o timing.o symbols.o decode.o -o bin/hopper


