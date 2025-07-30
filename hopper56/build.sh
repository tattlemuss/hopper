#!/usr/bin/env sh
set -e
set -x
CC=g++
LD=g++
CFLAGS="-std=c++11 -g  -Wextra -Wall -O0"
LDFLAGS="-lc"

# Just build everything -- this project isn't big
# Library (decoder) code
${CC} ${CFLAGS} -c -o decode56.o      lib/decode56.cpp
${CC} ${CFLAGS} -c -o instruction56.o lib/instruction56.cpp
# Application code
${CC} ${CFLAGS} -c -o main.o        main.cpp
${CC} ${CFLAGS} -c -o print.o       print.cpp
${CC} ${CFLAGS} -c -o symbols.o     symbols.cpp
# Link
${LD} ${LDFLAGS} main.o print.o symbols.o decode56.o instruction56.o -o hopper56

