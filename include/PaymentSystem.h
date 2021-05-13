/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : PaymentSystem.h
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 21/04/2021
*
* Purpose : declaración de funciones de PaymentSystem
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 25/04/2021    Tomás           1        Incluida librería string y añadido endlessloop
*
*
|********************************************************/

#ifndef SSOO_P3_PAYMENTSYSTEM_H
#define SSOO_P3_PAYMENTSYSTEM_H

#include "semaphoreI.h"
#include "definitions.h"
#include "colors.h"

/********************************* SEMAPHORES AND SHARED MEMORY MANAGEMENT ********************************************/
void get_shm_segments(int *shm_payment, struct TPayment **p_payment);
void get_sems(sem_t **p_sem_balance_ready, sem_t **p_sem_balance_charge);

/********************************* TASK MANAGEMENT ********************************************************************/
void get_and_process_task(sem_t *p_sem_balance_ready, sem_t *p_sem_balance_charge,
                          struct TPayment *p_payment);

#endif //SSOO_P3_PAYMENTSYSTEM_H
