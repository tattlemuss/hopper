SRC_PATH=.
OUT_PATH=./bin/hopper
CPP_SRC=$(wildcard ${SRC_PATH}/*.cpp)
CPP_BIN= $(CPP_SRC:${SRC_PATH}/%.cpp=${OUT_PATH}/%)
CC=g++
CFLAGS=-I${SRC_PATH}/lib -DDEBUG -std=c++11

all: $(OUT_PATH)

${OUT_PATH}: ${CPP_SRC}
	${CC} ${CFLAGS} $^ -g -o $@

clean:
	@rm -rf ${OUT_PATH}
