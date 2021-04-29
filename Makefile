CC = g++
EXEC = exec/
SOURCE = src/
CFLAGS = -Iinclude/ -lpthread -std=c++11 -lrt

all: directory ssooigle PaymentSystem

directory:
	mkdir -p ${EXEC}

ssooigle: ${SOURCE}semaphoreI.cpp ${SOURCE}Buscador.cpp
	${CC} $^ -o ${EXEC}$@ ${CFLAGS}

PaymentSystem: ${SOURCE}semaphoreI.cpp ${SOURCE}PaymentSystem.cpp
	${CC} $^ -o ${EXEC}$@ ${CFLAGS}

Prueba:
	${EXEC}ssooigle

clean:
	rm -rf ${EXEC}
