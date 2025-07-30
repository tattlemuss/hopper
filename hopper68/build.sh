#!/usr/bin/env sh
set -e
set -x
SRC_PATH=.
CC=g++
LD=g++
CFLAGS="-DDEBUG -std=c++11 -g  -Wextra -Wall -O0"
LDFLAGS="-lc"

# Just build everything -- this project isn't big
# lib code
${CC} ${CFLAGS} -c -o decode68.o      lib/decode68.cpp
${CC} ${CFLAGS} -c -o instruction68.o lib/instruction68.cpp
${CC} ${CFLAGS} -c -o timing68.o      lib/timing68.cpp

# Application code
${CC} ${CFLAGS} -c -o symbols.o     symbols.cpp
${CC} ${CFLAGS} -c -o print.o       print.cpp
${CC} ${CFLAGS} -c -o main.o        main.cpp

${LD} ${LDFLAGS} main.o print.o symbols.o instruction68.o timing68.o decode68.o -o hopper68


