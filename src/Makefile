CC=clang #g++
LD=clang #g++

SRC_PATH=.
OUT_PATH=./bin/hopper
CPP_SRC=./main.cpp

CPP_OBJ=$(CPP_SRC:${SRC_PATH}/%.cpp=${SRC_PATH}/%.o)
CFLAGS=-I${SRC_PATH}/lib -DDEBUG -std=c++11
LDFLAGS=-lstdc++

%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $^

# Final exe
${OUT_PATH}: ${CPP_OBJ}
	${LD} ${LDFLAGS} $^ -g -o $@

clean:
	@rm -rf *.o
	@rm -rf ${OUT_PATH}

all: $(OUT_PATH)
