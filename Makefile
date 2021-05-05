CC = g++
EXEC = exec/
SOURCE = src/
CFLAGS = -Iinclude/ -lpthread -std=c++11 -lrt

all: directory ssooigle PaymentSystem Client_Normal Client_Premium

directory:
	mkdir -p ${EXEC}

ssooigle: ${SOURCE}semaphoreI.cpp ${SOURCE}Buscador.cpp ${SOURCE}Client_Management.cpp
	${CC} $^ -o ${EXEC}$@ ${CFLAGS}

Client_Normal: ${SOURCE}Client_Normal ${SOURCE}mycmp.cpp ${SOURCE}Text.cpp ${SOURCE}Coincidence_Format.cpp
	${CC} $^ -o ${EXEC}$@ ${CFLAGS}

PaymentSystem: ${SOURCE}semaphoreI.cpp ${SOURCE}PaymentSystem.cpp
	${CC} $^ -o ${EXEC}$@ ${CFLAGS}

Client_Premium: ${SOURCE}semaphoreI.cpp ${SOURCE}Client_Premium.cpp ${SOURCE}Client_Normal.cpp ${SOURCE}Text.cpp
	${CC} $^ -o ${EXEC}$@ ${CFLAGS}

Prueba:
	${EXEC}ssooigle

clean:
	rm -rf ${EXEC}
