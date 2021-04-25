//
// Created by TOMIC on 25/04/2021.
//

#ifndef SSOO_P3_BUSCADOR_H
#define SSOO_P3_BUSCADOR_H

#include "definitions.h"
#include "semaphoreI.h"
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>

struct TProcess_t *g_process_table;

/* Process management */
void create_processes_by_class(enum ProcessClass_t clas, int n_processes, int index_process_table);
pid_t create_single_process(char *clas, char *path, char *argv);
void get_str_process_info(enum ProcessClass_t clas, char **path, char **str_process_class);


/* Semaphores and shared memory management */
void create_shm_segments(int *shm_payment, struct T_Payment **p_payment);
void create_sems(sem_t **sem_balance_ready, sem_t **sem_balance_charge);
void close_shared_memory_segments(int shm_payment);

void terminate_processes();
void free_resources();

#endif //SSOO_P3_BUSCADOR_H
