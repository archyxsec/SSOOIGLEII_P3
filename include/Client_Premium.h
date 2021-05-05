//
// Created by TOMIC on 28/04/2021.
//

#ifndef SSOO_P3_CLIENT_PREMIUM_H
#define SSOO_P3_CLIENT_PREMIUM_H

#include "Client_Normal.h"

void get_sems(sem_t **p_sem_request_ready, sem_t **p_sem_stored_request);
void get_shm_segments(int *shm_client, struct TRequest_t **p_request);

class Client_Premium : protected Client{
public:
    Client_Premium(int id, std::string word, std::string category);
};

#endif //SSOO_P3_CLIENT_PREMIUM_H
