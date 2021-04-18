CC = g++
EXEC = exec/
SOURCE = src/
CFLAGS = -Iinclude/ -lpthread -std=c++11

all: directory ssooigle

directory:
	mkdir -p ${EXEC}

ssooigle: ${SOURCE}${SOURCE}Main.cpp
	${CC} $^ -o ${EXEC}$@ ${CFLAGS}

clean:
	rm -rf ${EXEC}
