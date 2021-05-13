/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : Clients_Common.cpp
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 13/05/2021
*
* Purpose : funciones de Clients_Common, comunes a los clientes, básicamente obtener semáforos y segmentos de memoria
*
*
* Revision History:
*
* Date          Author          Ref      Revision
*
*
|********************************************************/

#include "../include/Clients_Common.h"

/********************************* SEMAPHORES AND SHARED MEMORY MANAGEMENT ********************************************/

void get_sems(sem_t **p_sem_request_ready, sem_t **p_sem_stored_request)
{
    *p_sem_request_ready = get_semaphore(SEM_REQUEST_READY);
    *p_sem_stored_request = get_semaphore(SEM_STORED_REQUEST);
}

void get_shm_segments(int *shm_client, struct TRequest_t **p_request)
{
    *shm_client = shm_open(SHM_CLIENT, O_RDWR, 0644);
    *p_request = static_cast<TRequest_t *>(mmap(nullptr, sizeof(struct TRequest_t),
                                                PROT_READ | PROT_WRITE, MAP_SHARED, *shm_client, 0));
}
