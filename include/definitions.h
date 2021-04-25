//
// Created by BlaZzes on 21/04/2021.
//

#ifndef SSOO_P3_DEFINITIONS_H
#define SSOO_P3_DEFINITIONS_H

#include <iostream>
#include <string>

#define SEM_BALANCE_READY "sem_balance_ready"
#define SEM_BALANCE_CHARGE "sem_balance_charge"
#define SHM_PAYMENT "shm_payment"
#define CLIENT_PATH "./exec/client"
#define CLIENT_CLASS "CLIENT"
#define PAYMENT_SYSTEM_PATH "./exec/PaymentSystem"
#define PAYMENT_SYSTEM_CLASS "PAYMENT_SYSTEM"
#define N_CLIENTS 5
#define N_PAYMENT_SYSTEM 1

enum ProcessClass_t {CLIENT, PAYMENT_SYSTEM};

struct TProcess_t {
    enum ProcessClass_t clas; /* DECODER or SYMBOL_DECODER */
    pid_t pid;                 /* Process ID */
    std::string str_process_class;   /* String representation of the process class */
};

struct T_Payment{
    int id;
    int client_initial_balance;
    int balance;
};

#endif //SSOO_P3_DEFINITIONS_H
