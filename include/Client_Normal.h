//
// Created by TOMIC on 28/04/2021.
//

#ifndef SSOO_P3_CLIENT_NORMAL_H
#define SSOO_P3_CLIENT_NORMAL_H

#include <iostream>
#include <string>
#include <vector>
#include "Client.h"

void get_sems(sem_t **p_sem_request_ready, sem_t **p_sem_stored_request);
void get_shm_segments(int *shm_client, struct TRequest_t **p_request);

class Client_Normal : protected Client{
private:
    int n_searches;

public:
    Client_Normal(int id, std::string word, std::string category, int n_searches);
    ~Client_Normal();
    int get_n_searches();

};

#endif //SSOO_P3_CLIENT_NORMAL_H
