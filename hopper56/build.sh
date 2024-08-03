#!/usr/bin/env sh
set -e
set -x
CC=g++
LD=g++
CFLAGS="-std=c++11 -g -Wall"
LDFLAGS="-lc"

# Just build everything -- this project isn't big
# Library (decoder) code
${CC} ${CFLAGS} -c -o decode.o      lib/decode.cpp
${CC} ${CFLAGS} -c -o instruction.o lib/instruction.cpp
# Application code
${CC} ${CFLAGS} -c -o main.o        main.cpp
${CC} ${CFLAGS} -c -o print.o       print.cpp
${CC} ${CFLAGS} -c -o symbols.o     symbols.cpp
# Link
${LD} ${LDFLAGS} main.o print.o decode.o instruction.o symbols.o -o hopper56



