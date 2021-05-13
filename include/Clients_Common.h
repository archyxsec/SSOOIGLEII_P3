/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : Clients_Common.h
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 13/05/2021
*
* Purpose : declaración de funciones comunes a todos los clientes
*
*
* Revision History:
*
* Date          Author          Ref      Revision
*
*
|********************************************************/

#ifndef SSOO_P3_CLIENTS_COMMON_H
#define SSOO_P3_CLIENTS_COMMON_H

#define _POSIX_SOURCE // for pause(2)

#include "definitions.h"
#include "semaphoreI.h"
#include "colors.h"

/********************************* SEMAPHORES AND SHARED MEMORY MANAGEMENT ********************************************/
void get_sems(sem_t **p_sem_request_ready, sem_t **p_sem_stored_request);
void get_shm_segments(int *shm_client, struct TRequest_t **p_request);

#endif //SSOO_P3_CLIENTS_COMMON_H
