#include<iostream>
#include<unistd.h>
#include "../include/Client_Premium.h"
#include "../include/definitions.h"
#include<sys/mman.h>
#include<fcntl.h>
#include <string>

#define LECTURA 0
#define ESCRITURA 1

Client_Premium::Client_Premium(int id, std::string word, std::string category) 
{
    this->_id = id;
    this->_word = word;
    this->_category = category;
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
    struct TRequest_t *client_premium;

    std::string word = argv[1];
    for(int i=2; i<argc; i++){
        if(fopen(argv[i],"r") != NULL){
            client_premium->v_Texts.push_back(Text(argv[i]));
        }
    }
    struct TRequest_t *client_premium;
    int shm_client;
    int p[2];

    pipe(p);
    close(p[LECTURA]);

    get_shm_segments(&shm_client, &client_premium);
    get_sems(&p_sem_request_ready, &p_sem_stored_request);

    wait_semaphore(p_sem_request_ready);
    client_premium->category = "Client_Premium";
    client_premium->word = word;
    client_premium->initial_balance = -1;
    client_premium->fd_descriptor = p[ESCRITURA];
    signal_semaphore(p_sem_stored_request);


    return 0;
}
