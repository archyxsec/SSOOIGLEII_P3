//
// Created by TOMIC on 28/04/2021.
//

#ifndef SSOO_P3_CLIENT_PREMIUM_LIMIT_H
#define SSOO_P3_CLIENT_PREMIUM_LIMIT_H


#include "../include/Client_Premium.h"

void get_sems(sem_t **p_sem_request_ready, sem_t **p_sem_stored_request);
void get_shm_segments(int *shm_client, struct TRequest_t **p_request);

class Client_Premium_Limit : public Client_Premium{
private:
    int balance;
public:
    Client_Premium_Limit(int id, std::string word, std::string category, int start_balance);
};

#endif //SSOO_P3_CLIENT_PREMIUM_LIMIT_H
