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

#define N_CLIENTS 5
#define N_PAYMENT_SYSTEM 1
#define CLIENT_PATH "./exec/client"
#define CLIENT_CLASS "CLIENT"
#define PAYMENT_SYSTEM_PATH "./exec/PaymentSystem"
#define PAYMENT_SYSTEM_CLASS "PAYMENT_SYSTEM"

int g_nProcesses = N_CLIENTS + N_PAYMENT_SYSTEM;
std::mutex mutex; // Mutex semaphore for critical seccion
std::vector<struct TProcess_t> v_clients; //vector of clients

/*Payment_system PID*/
pid_t payment_process;

/* Process management */
void create_clients(enum ProcessClass_t clas, int n_processes, int index_process_table);
void create_payment_system(enum ProcessClass_t clas);
pid_t create_single_process(std::string path, std::string clas, std::string argv);
void get_str_process_info(enum ProcessClass_t clas, std::string *path, std::string *str_process_class);


/* Semaphores and shared memory management */
void create_shm_segments(int *shm_payment, struct T_Payment **p_payment, int *shm_client, struct TRequest_t **p_request);
void create_sems(sem_t **sem_balance_ready, sem_t **sem_balance_charge);
void close_shared_memory_segments(int shm_payment);

void terminate_processes();
void free_resources();

#endif //SSOO_P3_BUSCADOR_H
