CC=g++
LD=g++

SRC_PATH=.
OUT_PATH=./bin/hopper
CPP_SRC=./main.cpp ./decode.cpp ./instruction.cpp ./symbols.cpp ./timing.cpp

CPP_OBJ=$(CPP_SRC:${SRC_PATH}/%.cpp=${SRC_PATH}/%.o)
CPP_DEPS=$(CPP_SRC:${SRC_PATH}/%.cpp=${SRC_PATH}/%.d)
CFLAGS=-I${SRC_PATH}/lib -std=c++11 -MMD -DDEBUG -g -Wall

LDFLAGS=-lstdc++ -g

%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $<

all: $(OUT_PATH)

# Include dependencies
-include ${CPP_DEPS}

# Final exe
${OUT_PATH}: ${CPP_OBJ}
	${LD} ${LDFLAGS} $^ -g -o $@

clean:
	@rm -rf *.o
	@rm -rf *.d
	@rm -rf ${OUT_PATH}
