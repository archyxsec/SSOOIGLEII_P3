//
// Created by TOMIC on 21/04/2021.
//

#ifndef SSOO_P3_PAYMENTSYSTEM_H
#define SSOO_P3_PAYMENTSYSTEM_H

#include "semaphoreI.h"
#include "definitions.h"

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <string>


void get_shm_segments(int *shm_payment, struct TPayment **p_payment);
void get_sems(sem_t **p_sem_balance_ready, sem_t **p_sem_balance_charge);

/* Task management */
void get_and_process_task(sem_t *p_sem_balance_ready, sem_t *p_sem_balance_charge,
                          struct TPayment *p_payment);


#endif //SSOO_P3_PAYMENTSYSTEM_H
