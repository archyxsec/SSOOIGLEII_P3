#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
#include "../include/Client_Premium_Limit.h"
#include "../include/definitions.h"

#define LECTURA 0
#define ESCRITURA 1


Client_Premium_Limit::Client_Premium_Limit(int id, std::string word, std::string category, int start_balance){
    this->_id = id;
    this->_word = word;
    this->_category = category;
    this->balance = balance;
}
void get_shm_segments(int *shm_client, struct TRequest_t **p_request)
{
    *shm_client = shm_open(SHM_CLIENT, O_RDWR, 0644);
    *p_request = static_cast<TRequest_t *>(mmap(NULL, sizeof(struct TRequest_t),
                                                PROT_READ | PROT_WRITE, MAP_SHARED, *shm_client, 0));
}

void get_sems(sem_t **p_sem_request_ready, sem_t **p_sem_stored_request)
{
    *p_sem_request_ready = get_semaphore(SEM_REQUEST_READY);
    *p_sem_stored_request = get_semaphore(SEM_STORED_REQUEST);
}


int main(int argc, char **argv){
    sem_t *p_sem_request_ready, *p_sem_stored_request;
    struct  TRequest_t *client_premium_limit;

    std::string word = argv[1];
    int balance = atoi(argv[2]);
    for(int i=3; i<argc; i++){
        if(fopen(argv[i],"r") != NULL){
            client_premium_limit->v_texts.push_back(Text(argv[i]));
        }
    }
    int shm_client;
    int p[2];

    pipe(p);
    close(p[ESCRITURA]);

    get_shm_segments(&shm_client, &client_premium_limit);
    get_sems(&p_sem_request_ready, &p_sem_stored_request);

    wait_semaphore(p_sem_request_ready);
    client_premium_limit->category = "Client_Premium_Limit";
    client_premium_limit->word = word;
    client_premium_limit->initial_balance = balance;
    client_premium_limit->fd_descriptor = p[ESCRITURA];
    signal_semaphore(p_sem_stored_request);


    return 0;
}
