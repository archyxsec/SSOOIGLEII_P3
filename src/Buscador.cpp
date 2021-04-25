#include "../include/Buscador.h"

int main() {

}
/* Process management */
void create_processes_by_class(enum ProcessClass_t clas, int n_processes, int index_process_table)
{
    char *path = NULL, *str_process_class = NULL;
    int i;
    pid_t pid;

    get_str_process_info(clas, &path, &str_process_class);

    for (i = index_process_table; i < (index_process_table + n_processes); i++) {
        pid = create_single_process(path, str_process_class, NULL);

        g_process_table[i].clas = clas;
        g_process_table[i].pid = pid;
        g_process_table[i].str_process_class = str_process_class;
    }

    printf("[Buscador] %d %s processes created.\n", n_processes, str_process_class);
    sleep(1);
}
pid_t create_single_process(char *clas, char *path, char *argv)
{
    pid_t pid;

    switch (pid = fork()) {
        case -1 :
            fprintf(stderr, "[MANAGER] Error creating %s process: %s.\n",
                    clas, strerror(errno));
            terminate_processes();
            free_resources();
            exit(EXIT_FAILURE);
            /* Child process */
        case 0 :
            if (execl(path, clas, argv, NULL) == -1) {
                fprintf(stderr, "[MANAGER] Error using execl() in %s process: %s.\n",
                        clas, strerror(errno));
                exit(EXIT_FAILURE);
            }
    }
}
void get_str_process_info(enum ProcessClass_t clas, char **path, char **str_process_class)
{
    switch (clas) {
        case CLIENT:
            *path = "./exec/client";
            *str_process_class =  "CLIENT";
        break;
        case PAYMENT_SYSTEM:
            *path = "./exec/payment_system";
            *str_process_class = "PAYMENT_SYSTEM";
        break;
    }
}


/* Semaphores and shared memory management */
void create_shm_segments(int *shm_payment, struct T_Payment **p_payment)
{
    /* Create and initialize shared memory segments */
    *shm_payment = shm_open("shm_payment", O_CREAT | O_RDWR, 0644);
    ftruncate(*shm_payment, sizeof(struct T_Payment));
    *p_payment = static_cast<T_Payment *>(mmap(NULL, sizeof(struct T_Payment),
            PROT_READ | PROT_WRITE, MAP_SHARED, *shm_payment, 0));
}
void create_sems(sem_t **sem_balance_ready, sem_t **sem_balance_charge)
{

}
void close_shared_memory_segments(int shm_payment)
{

}
void terminate_processes()
{

}
void free_resources()
{

}

