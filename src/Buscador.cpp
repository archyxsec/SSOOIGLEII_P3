#include "../include/Buscador.h"

int main(int argc, char **argv) {

    std::thread request_manager, queue_manager;
    int shm_payment, shm_client;
    sem_t *sem_balance_ready, *sem_balance_charge, *sem_request_ready, *sem_stored_request;
    struct T_Payment *payment;
    struct TRequest_t *request;
    create_shm_segments(&shm_payment, &payment,&shm_client, &request);
    create_sems(&sem_balance_ready, &sem_balance_charge, &sem_request_ready, &sem_stored_request);

    create_payment_system(PAYMENT_SYSTEM);

    /*We gonna simulate a simple Payment System*/
    /*payment->id = 0;
    payment->client_initial_balance = 10;
    payment->balance = 0;
    std::cout << "[BUSCADOR] manda recargar puntos un proceso con 0 de balance y 10 de initial_balance" << std::endl;
    signal_semaphore(sem_balance_ready);
    wait_semaphore(sem_balance_charge);
    std::cout << "[BUSCADOR] Saldo después de llamar a Payment_system = " << payment->balance << std::endl;
    */
    request_manager = std::thread(wait_requests, sem_request_ready, sem_stored_request, &request);
    queue_manager = std::thread(manage_queue);
    wait_requests(sem_request_ready, sem_stored_request,&request);

    return EXIT_SUCCESS;
}
void manage_queue() {
    int random_number;
    bool vip, choose;
    attend_repics:
    std::vector<std::string> v_texts;
    std::string word;
    int fd_write_client;
    int initial_balance;
    std::string category;
    for (;;) {
        std::unique_lock<std::mutex> ul(queue_semaphore_management);

        extract_request_condition.wait(ul, [] {
            return request_vector.size() > 0;
        });

        //Randomize number in order to choose 80% request for premium clients, and 20% normal client
        random_number = 1 + rand() % (10);
        vip = (random_number <= 8 ? true : false);
        int i;

        for (i = 0; i < request_vector.size(); i++) {
            if (vip && (request_vector[i].category == PREMIUM_CATEGORY ||
                        request_vector[i].category == ILIMITED_PREMIUM_CATEGORY))
                choose = true;
            else if (request_vector[i].category == NORMAL_CATEGORY) choose = true;

            if (choose) {
                category = request_vector[i].category;
                word = request_vector[i].word;
                fd_write_client = request_vector[i].fd_descriptor;
                initial_balance = request_vector[i].initial_balance;
                v_texts = request_vector[i].v_texts;
                break;
            }
        }
        request_vector.erase(request_vector.begin() + i); //Remove request for queue
        extract_request_condition.notify_one();
        /*Create process*/
        create_client_management(v_texts, word, fd_write_client, initial_balance, category);
        ul.unlock(); //Unlock the semaphore

    }
}
void create_client_management(std::vector<std::string> v_texts, std::string word,
                              int fd_write_client, int initial_balance, std::string category)
{
    pid_t pid;
    switch((pid = fork())){
        case -1:
            fprintf(stderr,"[BUSCADOR] Error while create a client_management.\n");
            std::exit(EXIT_FAILURE);
            break;
        case 0:
            if((execl(CLIENT_MANAGEMENT_PATH,"CLIENT_MANAGEMENT",category.c_str(), word.c_str(), initial_balance, fd_write_client, v_texts.)) == -1){

            }
    }
}

void wait_requests(sem_t *sem_request_ready, sem_t *sem_stored_request,struct TRequest_t **request){
    std::unique_lock<std::mutex> ul(queue_semaphore_management);

    for(;;){
        ul.block();
        signal_semaphore(sem_request_ready);
        wait_semaphore(sem_stored_request);
        request_vector.push_back(**request);
        cv.notify_one();
        ul.unlock();
    }
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

