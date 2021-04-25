#include "../include/Buscador.h"

int main(int argc, char **argv) {

    int shm_payment;
    sem_t *sem_balance_ready, *sem_balance_charge;
    struct T_Payment *payment;
    create_shm_segments(&shm_payment, &payment);
    create_sems(&sem_balance_ready, &sem_balance_charge);

    create_payment_system(PAYMENT_SYSTEM);
    std::cout << "Creado proceso system.." << std::endl;


    std::cout << "Creados Semaforos y segmentos" << std::endl;

    /*We gonna simulate a simple Payment System*/
    payment->id = 0;
    payment->client_initial_balance = 10;
    payment->balance = 0;
    std::cout << "[BUSCADOR] manda recargar puntos un proceso con 0 de balance y 10 de initial_balance" << std::endl;
    signal_semaphore(sem_balance_ready);
    wait_semaphore(sem_balance_charge);
    std::cout << "[BUSCADOR] Saldo después de llamar a Payment_system = " << payment->balance << std::endl;

    return EXIT_SUCCESS;
}
/* Process management */
void create_clients(enum ProcessClass_t clas, int n_processes, int index_process_table)
{
    struct TProcess_t process;
    int i;
    pid_t pid;

    process = get_str_process_info(clas);

    for (i = index_process_table; i < (index_process_table + n_processes); i++) {
        struct TProcess_t aux;
        pid = create_single_process(process, NULL);
        aux.pid = pid;
        aux.clas = process.clas;
        aux.str_process_class = process.str_process_class;
        v_clients.push_back(aux); // ad client to process
    }

    std::cout << "[Buscador] " << n_processes << " " << process.str_process_class << " processes created." << std::endl;
}
void create_payment_system(enum ProcessClass_t clas)
{
    struct TProcess_t process;
    int i;
    pid_t pid;

    process = get_str_process_info(clas);

    pid = create_single_process(process, NULL);

    /*PAYMENT SYSTEM CREATE IN BACKGROUND*/
    std::cout << "[Buscador] " << 1 << " " << process.str_process_class << " processes created." << std::endl;
    payment_process = pid;
}

pid_t create_single_process(struct TProcess_t process, std::string argv)
{
    pid_t pid;

    switch (pid = fork()) {
        case -1 :
            fprintf(stderr, "[MANAGER] Error creating %s process: %s.\n",
                    process.str_process_class.c_str(), strerror(errno));
            terminate_processes();
            free_resources();
            std::exit(EXIT_FAILURE);
            /* Child process */
        case 0 :
            if (execl(process.path.c_str(), process.str_process_class.c_str(), argv.c_str(), NULL) == -1) {
                fprintf(stderr, "[MANAGER] Error using execl() in %s process: %s.\n",
                        process.str_process_class.c_str(), strerror(errno));
                std::exit(EXIT_FAILURE);
            }
    }
    return pid;
}

struct TProcess_t get_str_process_info(enum ProcessClass_t clas)
{
    struct TProcess_t process;
    process.clas = clas;
    switch (clas) {
        case CLIENT:
            process.path = CLIENT_PATH;
            process.str_process_class =  CLIENT_CLASS;
        break;
        case PAYMENT_SYSTEM:
            process.path = PAYMENT_SYSTEM_PATH;
            process.str_process_class = PAYMENT_SYSTEM_CLASS;
            break;
    }
    return process;
}


/* Semaphores and shared memory management */
void create_shm_segments(int *shm_payment, struct T_Payment **p_payment)
{
    /* Create and initialize shared memory segments */
    *shm_payment = shm_open(SHM_PAYMENT, O_CREAT | O_RDWR, 0644);
    ftruncate(*shm_payment, sizeof(struct T_Payment));
    *p_payment = static_cast<T_Payment *>(mmap(NULL, sizeof(struct T_Payment),
            PROT_READ | PROT_WRITE, MAP_SHARED, *shm_payment, 0));
}
void create_sems(sem_t **sem_balance_ready, sem_t **sem_balance_charge)
{
    *sem_balance_ready = create_semaphore(SEM_BALANCE_READY,0);
    *sem_balance_charge = create_semaphore(SEM_BALANCE_CHARGE,0);
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

