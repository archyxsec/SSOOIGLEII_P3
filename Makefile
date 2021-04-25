CC = g++
EXEC = exec/
SOURCE = src/
CFLAGS = -Iinclude/ -lpthread -std=c++11

all: directory ssooigle

directory:
	mkdir -p ${EXEC}

ssooigle: ${SOURCE}semaphoreI.cpp ${SOURCE}PaymentSystem.cpp ${SOURCE}Buscador.cpp
	${CC} $^ -o ${EXEC}$@ ${CFLAGS}

clean:
	rm -rf ${EXEC}
