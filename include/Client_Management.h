//
// Created by TOMIC on 26/04/2021.
//

#ifndef SSOO_P3_CLIENT_MANAGEMENT_H
#define SSOO_P3_CLIENT_MANAGEMENT_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <exception>
#include <thread>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mutex>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Text.h"
#include "Coincidence_Format.h"
#include "colors.h"
#include "definitions.h"
#include "mycmp.h"
#include "semaphoreI.h"


std::mutex mutex;
std::priority_queue<Coincidence_Format, std::vector<Coincidence_Format>,
        myComp> coincidences;


void parse_argv(int argc, char **argv, std::vector<Text> &v_texts, char **word,
                char **pipename,int *initial_balance, char **category, int *client_pid);
void find_normal_client(int id, Text txt, int begin, int end, std::string pattern, int initial_balance);
void find_premium_client(int id, Text txt, int begin, int end, std::string pattern, int initial_balance);
void get_shm_segments(int *shm_payment, struct TPayment **p_payment);
void get_sems(sem_t **p_sem_balance_ready, sem_t **p_sem_balance_charge, sem_t **p_mutex);
void find_ilimited_premium_client(int id, Text txt, int begin, int end, std::string pattern);
void add_coincidence(Coincidence_Format coincidence);
void free_resources();
int get_number_coindicences();
void start_finding(std::vector<Text> v_texts, char *word,
                   char *pipename, int initial_balance, char *category, int client_pid);
std::string getCoincidences(int client_pid);
#endif //SSOO_P3_CLIENT_MANAGEMENT_H
