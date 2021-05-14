/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : definitions.h
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 21/04/2021
*
* Purpose : definiciones y constantes
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 25/04/2021    Tomás           1        Añadido TProcess_T y TPayment_T
* 25/04/2021    Tomás           2        incompatibilidad entre std::string y memoria compartida en C.
* 25/04/2021    Tomás           3        Definidos semáforos, clientes y cambiado TProcess_t
* 28/04/2021    Sergio          4        Definida estructura TRequest_t
* 29/04/2021    Tomás           5        Cambiado char* por std::string en estructuras
* 07/05/2021    Tomás           6        Añadidas constantes y cambio de std::String por char[] debido a que memoria compartida
*                                        de C no es compatible con std::string
*
|********************************************************/

#ifndef SSOO_P3_DEFINITIONS_H
#define SSOO_P3_DEFINITIONS_H

/* C Libraries */
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
/* C++11 Libraries */
#include <iostream>
#include <string>
#include <condition_variable>
#include <array>
#include <mutex>
#include <vector>
#include <thread>
#include <algorithm>
#include <random>
/* Self includes */
#include "Text.h"

#define SEM_BALANCE_READY "sem_balance_ready"
#define SEM_BALANCE_CHARGE "sem_balance_charge"
#define SEM_REQUEST_READY "sem_request_ready"
#define SEM_STORED_REQUEST "sem_stored_request"
#define SEM_REPLIC_FINISH "sem_replic_finish"
#define SEM_MUTEX "sem_mutex"
#define SHM_PAYMENT "shm_payment"
#define SHM_CLIENT "shm_client"
#define DATA_PATH "data/"
#define CLIENT_NORMAL_PATH "./exec/Client_Normal"
#define CLIENT_PREMIUM_PATH "./exec/Client_Premium"
#define CLIENT_PREMIUM_LIMIT_PATH "./exec/Client_Premium_Limit"
#define CLIENT_MANAGEMENT_PATH "./exec/Client_Management"
#define PAYMENT_SYSTEM_PATH "./exec/PaymentSystem"
#define PAYMENT_SYSTEM_CLASS "PAYMENT_SYSTEM"
#define ILIMITED_PREMIUM_CATEGORY "ILIMITED_PREMIUM"
#define PREMIUM_CATEGORY "PREMIUM"
#define NORMAL_CATEGORY "NORMAL"
#define N_CLIENTS 50
#define N_PAYMENT_SYSTEM 1
#define N_REPLICS 4
#define N_THREADS_PER_REPLIC 8
#define MAX_BUFFER_TEXT 1024
#define MAX_CREDITS 50


enum ProcessClass_t {ILIMITED_PREMIUM_CLIENT,PREMIUM_CLIENT,NORMAL_CLIENT, PAYMENT_SYSTEM};

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
    char pipename[MAX_BUFFER_TEXT];
    char v_texts[MAX_BUFFER_TEXT];
};

struct TPayment{
    int id;
    int client_initial_balance;
    int balance;
};

#endif //SSOO_P3_DEFINITIONS_H
