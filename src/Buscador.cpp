#include "../include/Buscador.h"

int main(int argc, char **argv) {

    int shm_payment, shm_client;
    sem_t *sem_balance_ready, *sem_balance_charge, *sem_request_ready, *sem_stored_request;
    struct T_Payment *payment;
    struct TRequest_t *request;
    std::vector<TRequest_t> request_Vector;

    create_shm_segments(&shm_payment, &payment,&shm_client, &request);
    create_sems(&sem_balance_ready, &sem_balance_charge, &sem_request_ready, &sem_stored_request);

    create_payment_system(PAYMENT_SYSTEM);

    /*Esto seria una idea de como almacenar las peticiones, falta debatirlo para ver si esta bien planteado*/
    //DUDA--> HABRIA QUE HACER UN FOR CON LOS CLIENTES PARA ALMACENAR TODAS LAS PETICIONES O SE EJECUTA DENTRO DE UN WHILE 1 ESTE MAIN

    signal_semaphore(sem_request_ready);
    wait_semaphore(sem_stored_request);
    request_Vector.push_back(*request);
 

    /*We gonna simulate a simple Payment System*/
    /*payment->id = 0;
    payment->client_initial_balance = 10;
    payment->balance = 0;
    std::cout << "[BUSCADOR] manda recargar puntos un proceso con 0 de balance y 10 de initial_balance" << std::endl;
    signal_semaphore(sem_balance_ready);
    wait_semaphore(sem_balance_charge);
    std::cout << "[BUSCADOR] Saldo después de llamar a Payment_system = " << payment->balance << std::endl;*/

    return EXIT_SUCCESS;
}
/* Process management */
void create_clients(enum ProcessClass_t clas, int n_processes, int index_process_table)
{
    std::string *path = new std::string();
    std::string *str_process_class = new std::string();
    int i;
    pid_t pid;

    get_str_process_info(clas, path, str_process_class);

    for (i = index_process_table; i < (index_process_table + n_processes); i++) {
        pid = create_single_process(path->data(), str_process_class->data(), "");
        struct TProcess_t process;
        process.clas = clas;
        process.pid = pid;
        process.str_process_class = str_process_class->data();
        v_clients.push_back(process); // ad client to process
    }

    std::cout << "[Buscador] " << n_processes << " " << str_process_class->data() << " processes created." << std::endl;
}
void create_payment_system(enum ProcessClass_t clas)
{
    std::string *path = new std::string();
    std::string *str_process_class = new std::string();
    int i;
    pid_t pid;

    get_str_process_info(clas, path, str_process_class);

    pid = create_single_process(path->data(), str_process_class->data(), "");

    /*PAYMENT SYSTEM CREATE IN BACKGROUND*/
    std::cout << "[Buscador] " << 1 << " " << str_process_class->data() << " processes created." << std::endl;
    payment_process = pid;
}

pid_t create_single_process(std::string path, std::string clas, std::string argv)
{
    pid_t pid;

    switch (pid = fork()) {
        case -1 :
            fprintf(stderr, "[MANAGER] Error creating %s process: %s.\n",
                    clas.c_str(), strerror(errno));
            terminate_processes();
            free_resources();
            std::exit(EXIT_FAILURE);
            /* Child process */
        case 0 :
            if (execl(path.c_str(), clas.c_str(), argv.c_str(), NULL) == -1) {
                fprintf(stderr, "[MANAGER] Error using execl() in %s process: %s.\n",
                        clas.c_str(), strerror(errno));
                std::exit(EXIT_FAILURE);
            }
    }
    return pid;
}

void get_str_process_info(enum ProcessClass_t clas, std::string *path, std::string *str_process_class)
{
    switch (clas) {
        case CLIENT:
            *path = CLIENT_PATH;
            *str_process_class =  CLIENT_CLASS;
        break;
        case PAYMENT_SYSTEM:
            *path = PAYMENT_SYSTEM_PATH;
            *str_process_class = PAYMENT_SYSTEM_CLASS;
        break;
    }
}


/* Semaphores and shared memory management */
void create_shm_segments(int *shm_payment, struct T_Payment **p_payment, int *shm_client, struct TRequest_t **p_request)
{
    /* Create and initialize shared memory segments */
    *shm_payment = shm_open("shm_payment", O_CREAT | O_RDWR, 0644);
    ftruncate(*shm_payment, sizeof(struct T_Payment));
    *p_payment = static_cast<T_Payment *>(mmap(NULL, sizeof(struct T_Payment),
            PROT_READ | PROT_WRITE, MAP_SHARED, *shm_payment, 0));

    *shm_client = shm_open("shm_client", O_CREAT | O_RDWR, 0644);
    ftruncate(*shm_client, sizeof(struct TRequest_t));
    *p_request = static_cast<TRequest_t *>(mmap(NULL, sizeof(struct TRequest_t),
            PROT_READ | PROT_WRITE, MAP_SHARED, *shm_client, 0));
}
void create_sems(sem_t **sem_balance_ready, sem_t **sem_balance_charge, sem_t **sem_request_ready, sem_t **sem_stored_request)
{
    *sem_balance_ready = create_semaphore(SEM_BALANCE_READY,0);
    *sem_balance_charge = create_semaphore(SEM_BALANCE_CHARGE,0);
    *sem_request_ready = create_semaphore(SEM_REQUEST_READY,0);
    *sem_stored_request = create_semaphore(SEM_STORED_REQUEST,0);
}
void close_shared_memory_segments(int shm_payment)
{
    close(shm_payment);
}
void terminate_processes()
{
    int i;

    /*First We kill the PaymentSystem Process*/
    if (kill(payment_process, SIGINT) == -1) {
        fprintf(stderr, "[BUSCADOR] Error using kill() on process %d: %s.\n",
                payment_process, strerror(errno));
    }
    /*Next We kill Clients Processes*/
    std::cout << "\n----- [BUSCADOR] Terminating running child processes ----- " << std::endl;
    for (i = 0; i < v_clients.size(); i++) {
        /* Child process alive */
        if (v_clients[i].pid != 0) {
            std::cout << "[BUSCADOR] Terminating " << v_clients[i].str_process_class <<
            " process [" << v_clients[i].pid << "]..." << std::endl;
            if (kill(v_clients[i].pid, SIGINT) == -1) {
                fprintf(stderr, "[BUSCADOR] Error using kill() on process %d: %s.\n",
                        v_clients[i].pid, strerror(errno));
            }
        }
    }
}
void free_resources()
{
    std::cout << "\n----- [BUSCADOR] Freeing resources ----- " << std::endl;

    v_clients.clear();

    /* Semaphores */
    remove_semaphore(SEM_BALANCE_READY);
    remove_semaphore(SEM_BALANCE_CHARGE);

    /* Shared memory segments*/
    shm_unlink(SHM_PAYMENT);
}

