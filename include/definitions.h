//
// Created by BlaZzes on 21/04/2021.
//

#ifndef SSOO_P3_DEFINITIONS_H
#define SSOO_P3_DEFINITIONS_H

#include <iostream>
#include <string>
#include <condition_variable>
#include <signal.h>
#include <string.h>
#include <array>
#include "Text.h"

#define SEM_BALANCE_READY "sem_balance_ready"
#define SEM_BALANCE_CHARGE "sem_balance_charge"
#define SEM_REQUEST_READY "sem_request_ready"
#define SEM_STORED_REQUEST "sem_stored_request"
#define SHM_PAYMENT "shm_payment"
#define SHM_CLIENT "shm_client"
#define CLIENT_PATH "./exec/client"
#define CLIENT_PREMIUM_PATH "./exec/Client_Premium"
#define CLIENT_MANAGEMENT_PATH "./exec/Client_Management"
#define CLIENT_CLASS "CLIENT"
#define PAYMENT_SYSTEM_PATH "./exec/PaymentSystem"
#define PAYMENT_SYSTEM_CLASS "PAYMENT_SYSTEM"
#define ILIMITED_PREMIUM_CATEGORY "ILIMITED_PREMIUM"
#define PREMIUM_CATEGORY "PREMIUM"
#define NORMAL_CATEGORY "NORMAL"
#define N_CLIENTS 50
#define N_PAYMENT_SYSTEM 1
#define N_REPLICS 4
#define N_THREADS_PER_REPLIC 8
#define BUFFER_RESULTS 5000
#define MAX_BUFFER_TEXT 1024


enum ProcessClass_t {CLIENT, PAYMENT_SYSTEM};

struct TProcess_t {
    enum ProcessClass_t clas;
    pid_t pid;                 /* Process ID */
    std::string str_process_class;   /* String representation of the process class */
};

struct TRequest_t{
    int client_pid;
    char category[MAX_BUFFER_TEXT];
    char word[MAX_BUFFER_TEXT];
    int initial_balance;
    int fd_descriptor;
    char v_texts[MAX_BUFFER_TEXT];
};

struct T_Payment{
    int id;
    int client_initial_balance;
    int balance;
};

#endif //SSOO_P3_DEFINITIONS_H
