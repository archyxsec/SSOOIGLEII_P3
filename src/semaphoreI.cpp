/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : semaphoreI.cpp
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 21/04/2021
*
* Purpose : abstracción de funciones para manejo de semáforos
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 25/04/2021    Tomás           1        Añadido include
* 13/05/2021    Tomás           2        Cambiado parametros de funciones por const std::string&
*
|********************************************************/

#include "../include/semaphoreI.h"

sem_t *create_semaphore (const std::string& name, unsigned int value) {
    sem_t *sem;

    if ((sem = sem_open(name.c_str(), O_CREAT, 0644, value)) == SEM_FAILED) {
        fprintf(stderr, "Error creating semaphore : %s\n", strerror(errno));
        std::exit(EXIT_FAILURE);
    }

    return sem;
}

sem_t *get_semaphore (const std::string& name) {
    sem_t *sem;

    if ((sem = sem_open(name.c_str(), O_RDWR)) == SEM_FAILED) {
        fprintf(stderr, "Error retrieving semaphore <%s>: %s\n", name.c_str(), strerror(errno));
        std::exit(EXIT_FAILURE);
    }

    return sem;
}

void remove_semaphore (const std::string& name) {
    sem_t *sem = get_semaphore(name);

    if ((sem_close(sem)) == -1) {
        fprintf(stderr, "Error closing semaphore: %s\n", strerror(errno));
        std::exit(EXIT_FAILURE);
    }

    if ((sem_unlink(name.c_str())) == -1) {
        fprintf(stderr, "Error removing semaphore: %s\n", strerror(errno));
        std::exit(EXIT_FAILURE);
    }
}

void signal_semaphore (sem_t *sem) {
    if ((sem_post(sem)) == -1) {
        fprintf(stderr, "Error incrementing the semaphore: %s\n", strerror(errno));
        std::exit(EXIT_FAILURE);
    }
}

void wait_semaphore (sem_t *sem) {
    if ((sem_wait(sem)) == -1) {
        fprintf(stderr, "Error decrementing the semaphore: %s\n", strerror(errno));
        std::exit(EXIT_FAILURE);
    }
}

