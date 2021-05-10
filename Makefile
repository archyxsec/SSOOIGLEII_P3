CC = g++
EXEC = exec/
SOURCE = src/
CFLAGS = -Iinclude/ -lpthread -std=c++11 -lrt

all: directory ssooigle PaymentSystem Client_Management Client_Premium Client_Premium_Limit Client_Normal

directory:
	mkdir -p ${EXEC}

ssooigle: ${SOURCE}Buscador.cpp ${SOURCE}semaphoreI.cpp
	${CC} $^ -o ${EXEC}$@ ${CFLAGS}

Client_Management: ${SOURCE}Client_Management.cpp ${SOURCE}mycmp.cpp ${SOURCE}Text.cpp ${SOURCE}Coincidence_Format.cpp ${SOURCE}semaphoreI.cpp
	${CC} $^ -o ${EXEC}$@ ${CFLAGS}

Client_Premium: ${SOURCE}Client_Premium.cpp ${SOURCE}mycmp.cpp ${SOURCE}Text.cpp ${SOURCE}Coincidence_Format.cpp ${SOURCE}semaphoreI.cpp
	${CC} $^ -o ${EXEC}$@ ${CFLAGS}

Client_Premium_Limit: ${SOURCE}Client_Premium_Limit.cpp ${SOURCE}mycmp.cpp ${SOURCE}Text.cpp ${SOURCE}Coincidence_Format.cpp ${SOURCE}semaphoreI.cpp
	${CC} $^ -o ${EXEC}$@ ${CFLAGS}

Client_Normal: ${SOURCE}Client_Normal.cpp ${SOURCE}mycmp.cpp ${SOURCE}Text.cpp ${SOURCE}Coincidence_Format.cpp ${SOURCE}semaphoreI.cpp
	${CC} $^ -o ${EXEC}$@ ${CFLAGS}

PaymentSystem: ${SOURCE}PaymentSystem.cpp ${SOURCE}semaphoreI.cpp
	${CC} $^ -o ${EXEC}$@ ${CFLAGS}

Prueba:
	${EXEC}ssooigle

clean:
	rm -rf ${EXEC}
