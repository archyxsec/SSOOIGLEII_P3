//
// Created by TOMIC on 25/04/2021.
//

#ifndef SSOO_P3_BUSCADOR_H
#define SSOO_P3_BUSCADOR_H

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <mutex>
#include <vector>
#include <string>

#include "semaphoreI.h"
#include "definitions.h"

int g_nProcesses = N_CLIENTS + N_PAYMENT_SYSTEM;
std::mutex mutex; // Mutex semaphore for critical seccion
std::vector<struct TProcess_t> v_clients; //vector of clients

/*Payment_system PID*/
pid_t payment_process;

/* Process management */
void create_clients(enum ProcessClass_t clas, int n_processes, int index_process_table);
void create_payment_system(enum ProcessClass_t clas);
pid_t create_single_process(struct TProcess_t process, std::string argv);
struct TProcess_t get_str_process_info(enum ProcessClass_t clas);


/* Semaphores and shared memory management */
void create_shm_segments(int *shm_payment, struct T_Payment **p_payment);
void create_sems(sem_t **sem_balance_ready, sem_t **sem_balance_charge);
void close_shared_memory_segments(int shm_payment);

void terminate_processes();
void free_resources();

#endif //SSOO_P3_BUSCADOR_H
