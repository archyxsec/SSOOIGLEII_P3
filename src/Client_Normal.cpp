//
// Created by TOMIC on 28/04/2021.
//

#include "../include/Client_Normal.h"

#define LECTURA 0
#define ESCRITURA 1

int main(int argc, char **argv){
    sem_t *p_sem_request_ready, *p_sem_stored_request;
    struct  TRequest_t *client_normal;

    std::string word = argv[1];
    int n_searchs = atoi(argv[2]);
    for(int i=3; i<argc; i++){
        if(fopen(argv[i],"r") != NULL){
            client_normal->v_texts.push_back(Text(argv[i]));
        }
    }
    int shm_client;
    int p[2];

    pipe(p);
    close(p[ESCRITURA]);

    get_shm_segments(&shm_client, &client_normal);
    get_sems(&p_sem_request_ready, &p_sem_stored_request);

    wait_semaphore(p_sem_request_ready);
    client_normal->category = "Client_Normal";
    client_normal->word = word;
    client_normal->initial_balance=n_searchs;
    client_normal->fd_descriptor=p[ESCRITURA];
    signal_semaphore(p_sem_stored_request);



    return 0;
}
Client_Normal::Client_Normal(int id, std::string word, std::string category, int n_searches) : Client(id, word, category) {
    this->n_searches = n_searches;
}
Client_Normal::~Client_Normal() {
}
int Client_Normal::get_n_searches() {
    return this->n_searches;
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
