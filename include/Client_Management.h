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

#include "Text.h"
#include "Coincidence_Format.h"
#include "colors.h"
#include "definitions.h"
#include "mycmp.h"


std::mutex mutex;
std::priority_queue<Coincidence_Format, std::vector<Coincidence_Format>,
        myComp> coincidences;


void parse_argv(int argc, char **argv, std::vector<Text> &v_texts, std::string *word,
                int *fd_write_client,int *initial_balance, std::string *category, int *client_pid);
void find_ilimited_premium_client(int id, Text txt, int begin, int end, std::string pattern);
void add_coincidence(Coincidence_Format coincidence);
void free_resources();
int get_number_coindicences();
void start_finding(std::vector<Text> v_texts, std::string word,int fd_write_client,
                   int initial_balance, std::string category, int client_pid);
std::string getCoincidences(int fd_descriptor);


#endif //SSOO_P3_CLIENT_MANAGEMENT_H
