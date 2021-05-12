/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : Client_Management.h
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 21/04/2021
*
* Purpose : declaración de funciones del manejador de clientes
*
*
* Revision History:
*
* Date          Author          Ref      Revision
*
*
|********************************************************/

#ifndef SSOO_P3_CLIENT_MANAGEMENT_H
#define SSOO_P3_CLIENT_MANAGEMENT_H

/* C++11 Libraries neccesaries for this source*/
#include <queue>
#include <exception>
#include <stdlib.h>
#include <time.h>
#include <future>

#include "Text.h"
#include "Coincidence_Format.h"
#include "colors.h"
#include "mycmp.h"
#include "semaphoreI.h"
#include "definitions.h" // Common libraries and definitions

std::mutex mutex; // Mutex for add coincidences
std::priority_queue<Coincidence_Format, std::vector<Coincidence_Format>,
        myComp> coincidences;

/********************************** ARGUMENT MANAGEMENT *************************************************/
void parse_argv(int argc, char **argv, std::vector<Text> &v_texts, char **word,
                char **pipename,int *initial_balance, char **category, int *client_pid);

/*********************************** SEARCH MANAGEMENT **************************************************/
void start_finding(std::vector<Text> v_texts, char *word,
                   char *pipename, int initial_balance, char *category, int client_pid);
void find_normal_client(int id, Text txt, int begin, int end, std::string pattern,
                        int initial_balance, std::promise<int>& prom);
void find_premium_client(int id, Text txt, int begin, int end, std::string pattern,
                         int initial_balance, std::promise<int>& prom);
void find_ilimited_premium_client(int id, Text txt, int begin, int end, std::string pattern, std::promise<int>& prom);

/****************************** COINCIDENCE MANAGEMENT ****************************************/
void add_coincidence(Coincidence_Format coincidence);
int get_number_coindicences();
std::string getCoincidences(int client_pid);

/********************************* SEMAPHORES AND SHARED MEMORY MANAGEMENT ********************************************/
void get_shm_segments(int *shm_payment, struct TPayment **p_payment);
void get_sems(sem_t **p_sem_balance_ready, sem_t **p_sem_balance_charge, sem_t **p_mutex);

/******** FREE RESOURCES *********/
void free_resources();
#endif //SSOO_P3_CLIENT_MANAGEMENT_H
