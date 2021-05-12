/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : Buscador.cpp
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 21/04/2021
*
* Purpose : Programa principal Buscador
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 25/04/2021    Tomás           1        incompatibilidad entre std::string y memoria compartida en C.
*
*
|********************************************************/

#include "../include/Buscador.h"

int main(int argc, char **argv) {

    std::vector<std::thread> t_requests_managers;
    int shm_payment, shm_client;
    sem_t *sem_balance_ready, *sem_balance_charge, *sem_request_ready, *sem_stored_request, *sem_replic_finish;
    struct T_Payment *payment;
    struct TRequest_t *request;
    install_signal_handler(SIGINT); /*Signal for client_management end in order to decrement n_replics*/
    create_shm_segments(&shm_payment, &payment,&shm_client, &request);
    create_sems(&sem_balance_ready, &sem_balance_charge, &sem_request_ready, &sem_stored_request, &sem_replic_finish);
    create_payment_system(PAYMENT_SYSTEM);

    t_requests_managers.emplace_back(wait_requests, sem_request_ready, sem_stored_request, request);
    t_requests_managers.emplace_back(manage_queue);
    t_requests_managers.emplace_back(manage_clients_management_termination,sem_replic_finish);

    /*Create the clients*/
    create_aleatory_clients(N_CLIENTS);

    std::for_each(t_requests_managers.begin(), t_requests_managers.end(), [](std::thread& t) { t.join(); });
    close_shared_memory_segments(shm_payment, shm_client);
    terminate_processes();
    free_resources();
    return EXIT_SUCCESS;
}

/******************************** CLIENT REQUESTS MANAGEMEN *****************************************************/

[[noreturn]] void manage_clients_management_termination(sem_t *sem_replic_finish){
    for(;;){
        wait_semaphore(sem_replic_finish);
        n_replics--;
        total_clients_attends++;
        std::cout << "[THREAD MANAGER FOR CLIENTS TERMINATION] n_replics decremented. N_replics = " << n_replics << std::endl;
        std::cout << "[THREAD MANAGER FOR CLIENTS TERMINATION] clients satisfied = " << total_clients_attends << std::endl;
        std::cout << "[THREAD MANAGER FOR CLIENTS TERMINATION] request vector size= " << request_vector.size() << std::endl;

        if(total_clients_attends == N_CLIENTS){
            std::cout << "[BUSCADOR] All clients satisfied" << std::endl;
            terminate_processes();
            free_resources();
            std::exit(EXIT_SUCCESS);
        }
        extract_request_condition.notify_all();
    }
}

[[noreturn]] void wait_requests(sem_t *sem_request_ready, sem_t *sem_stored_request,struct TRequest_t *request){

    for(;;){
        std::unique_lock<std::mutex> ul(queue_semaphore_management);

        signal_semaphore(sem_request_ready);
        wait_semaphore(sem_stored_request);
        std::cout << BOLDWHITE << "[BUSCADOR WAIT REQUESTS THREAD] " << RESET << RED << request->client_pid << RESET
                  << " client request accepted" << std::endl;
        request_vector.push_back(*request);
        extract_request_condition.notify_one();
        extract_request_condition.wait(ul);
        //extract_request_condition.wait(ul);

        //std::unique_lock<std::mutex> ul2(termination_client_management);
        //n_clients_attend.wait(ul,[]{return total_clients_attends < N_CLIENTS;});
    }
}

[[noreturn]] void manage_queue() {
    bool vip, choose;
    int random_number;
    char v_texts[MAX_BUFFER_TEXT];
    char word[MAX_BUFFER_TEXT];
    char pipename[MAX_BUFFER_TEXT];
    int initial_balance;
    char category[MAX_BUFFER_TEXT];
    int client_pid;

    for (;;) {

        std::unique_lock<std::mutex> ul(queue_semaphore_management);

        extract_request_condition.wait(ul, [] {
            return (request_vector.size() > 0) && ((n_replics < N_REPLICS) && (n_replics >=0));
        });
        std::cout << BOLDWHITE << "[HILO MANAGE QUEUE] Extract a request" << RESET<< std::endl;
        choose = false;
        std::cout << BOLDWHITE << "[HILO MANAGE QUEUE] request vector size = " << request_vector.size() << RESET << std::endl;


        //Randomize number in order to choose 80% request for premium clients, and 20% normal client
        random_number = 1 + rand() % (10);
        vip = random_number <= 8;
        int i;
        for (i = 0; i < request_vector.size(); i++) {

            if (vip && (strncmp(request_vector[i].category,PREMIUM_CATEGORY,sizeof (request_vector[i].category))==0 ||
                    strncmp(request_vector[i].category,ILIMITED_PREMIUM_CATEGORY,sizeof (request_vector[i].category))==0 ))
                choose = true;
            else if (strncmp(request_vector[i].category,NORMAL_CATEGORY,sizeof (request_vector[i].category)) == 0) choose = true;
            if (choose) {
                strcpy(category,request_vector[i].category );
                strcpy(word,request_vector[i].word );
                strcpy(pipename,request_vector[i].pipename);
                initial_balance = request_vector[i].initial_balance;
                strcpy(v_texts,request_vector[i].v_texts);
                client_pid = request_vector[i].client_pid;
                request_vector.erase(request_vector.begin() + i); //Remove request for queue
                create_client_management(v_texts, word,pipename, initial_balance, category, client_pid);
                n_replics++;
                std::cout << "[HILO MANAGE QUEUE] n_replics: " << n_replics << std::endl;
                break;
            }
        }
        extract_request_condition.notify_one();
   }
}
void create_client_management(char *v_texts, char *word,
                              char *pipename, int initial_balance, char *category, int client_pid)
{
    pid_t pid;
    char initial_balance_str[MAX_BUFFER_TEXT];
    char client_pid_str[MAX_BUFFER_TEXT];
    int v_text_len;
    char *token;
    int argv_len;
    int j;
    char auxiliar_buffer[MAX_BUFFER_TEXT];
    switch((pid = fork())){
        case -1:
            fprintf(stderr,"%s[BUSCADOR] Error while create a client_management.%s\n",RED,RESET);
            free_resources();
            std::exit(EXIT_FAILURE);
        case 0:
            /*Format integer to char* or string*/
            sprintf(initial_balance_str,"%i",initial_balance);
            sprintf(client_pid_str,"%d",client_pid);

            /*Get the number of texts*/
            strncpy(auxiliar_buffer, v_texts,sizeof (v_texts)); // Copy v_text in auxilizar buffer because tokenizer delete elements
            v_text_len = get_text_len(auxiliar_buffer);
            /*Prepare the argv vector to execl*/
            argv_len = v_text_len + 7; //7 parameters, without the texts
            char *argv[argv_len];
            argv[0] = (char *)ILIMITED_PREMIUM_CATEGORY;
            argv[1] = category;
            argv[2] = word;
            argv[3] = initial_balance_str;
            argv[4] = pipename;
            argv[5] = client_pid_str;
            j = 6;
            /*Tokenizer and get the file_name of the texts*/
            token = strtok(v_texts,"-");
            while(token!=nullptr){
                argv[j++] = token;
                token = strtok(nullptr, "-");
            }
            /*Finnish with NULL argv*/
            argv[j] = nullptr;

            /*Finnish with execute the client manager*/
            if((execv(CLIENT_MANAGEMENT_PATH,argv)) == -1){
                fprintf(stderr,"%s[BUSCADOR] Error while create Client Management.%s\n",RED,RESET);
                std::exit(EXIT_FAILURE);
            }
    }
}

int get_text_len(char *v_texts){
    /*Clients send the texts separated by '-'*/
    int j = 0;
    char *token;
    token = strtok(v_texts,"-");
    while(token!=nullptr){
        j++;
        token = strtok(nullptr, "-");
    }
    return j;
}

/**************************** PROCESSES MANAGEMENT **********************************************/

void create_payment_system(enum ProcessClass_t clas)
{
    auto *path = new std::string();
    auto *str_process_class = new std::string();
    int i;
    pid_t pid;
    char *argv[2];

    get_str_process_info(clas, path, str_process_class);
    argv[0] = (char *)PAYMENT_SYSTEM_CLASS;
    argv[1] = NULL;

    pid = create_single_process(*path,*str_process_class,argv);

    /*PAYMENT SYSTEM CREATE IN BACKGROUND*/
    std::cout << "[BUSCADOR] " << 1 << BOLDCYAN << " "
        << str_process_class->data() << RESET << " processes created." << std::endl;
    payment_process = pid;
}
void create_aleatory_clients(int n_clients)
{
    DIR *dir;
    struct dirent *ent;
    char FileNameBuffer[MAX_BUFFER_TEXT];
    char AuxiliarBuffer[MAX_BUFFER_TEXT];
    char possible_words[8][MAX_BUFFER_TEXT] = {"hola","lol","vive","sueño","lól","LOL","prueba","sirena"};
    char File_texts[MAX_BUFFER_TEXT][MAX_BUFFER_TEXT];
    char *word;
    int random_path_number, random_number_texts, random_word_number, random_text_index, random_credits_number;
    int i;
    pid_t pid;
    int file_text_elements = 0;

    if ((dir = opendir (DATA_PATH)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0) continue;
            sprintf(FileNameBuffer,"%s%s",DATA_PATH,ent->d_name); // Build the full file path
            /*Consider that all files are file mode not directories because We create de data/ directory*/
            strcpy(File_texts[file_text_elements],FileNameBuffer);
            file_text_elements++;
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        std::exit(EXIT_FAILURE);
    }

    for (i = 0; i < n_clients; i++) {
        int argv_index = 0;
        auto *path = new std::string();
        auto *str_process_class = new std::string();
        enum ProcessClass_t clas;
        int n_argvs;
        std::vector<int> previous_indexes;
        random_path_number = rand() % 3; // for {ILIMITED_PREMIUM_CLIENT,PREMIUM_CLIENT,NORMAL_CLIENT};
        random_word_number = rand() % (8);
        random_number_texts = 1 + rand() % (file_text_elements-3); // Max 7 texts

        if(random_path_number == 0) {
            n_argvs = 3 + random_number_texts; // class name, word, NULL and random_number_texts
            clas = ILIMITED_PREMIUM_CLIENT;
        }else if(random_path_number == 1){
            n_argvs = 4 + random_number_texts; // class name, word, credits, NULL and random_number_texts
            clas = PREMIUM_CLIENT;
        }
        else {
            n_argvs = 4 + random_number_texts; // class name, word, credits, NULL and random_number_texts
            clas = NORMAL_CLIENT;
        }
        char *argv[n_argvs];

        if(clas == ILIMITED_PREMIUM_CLIENT) argv[argv_index++] = (char *)ILIMITED_PREMIUM_CATEGORY;
        else if(clas == PREMIUM_CLIENT) argv[argv_index++] = (char *)PREMIUM_CATEGORY;
        else argv[argv_index++] = (char *)NORMAL_CATEGORY;

        word = possible_words[random_word_number]; // take the random word
        argv[argv_index++] = word;

        if(clas == PREMIUM_CLIENT || clas == NORMAL_CLIENT) {
            random_credits_number = 1 + rand() % 50;
            sprintf(AuxiliarBuffer,"%d",random_credits_number);
            argv[argv_index++] = AuxiliarBuffer;
        }

        /* Build the texts arguments */
        for(int j=0;j<random_number_texts;j++){
            bool find = false;
            random_text_index = rand() % (file_text_elements);
            while(!find) {
                if(!(std::find(previous_indexes.begin(), previous_indexes.end(), random_text_index) != previous_indexes.end())){
                    find = true;
                    argv[argv_index] = File_texts[random_text_index];
                    previous_indexes.push_back(random_text_index);
                    argv_index++;
                }else{
                    random_text_index = rand() % (file_text_elements);
                }
            }
        }

        argv[argv_index] = NULL;

        get_str_process_info(clas, path, str_process_class);
        pid = create_single_process(*path, *str_process_class, static_cast<char **>(argv));
        struct TProcess_t process;
        process.clas = clas;
        process.pid = pid;
        process.str_process_class = *str_process_class;
        v_clients.push_back(process); // ad client to process
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Process " << process.str_process_class << " Created with following arguments:" << std::endl;
        for(int x=0;x<argv_index; x++) std::cout << argv[x] << " ";
        std::cout << std::endl;
        extract_request_condition.notify_all(); // Notify manague queue
    }

    std::cout << BOLDWHITE << "[BUSCADOR] " << RESET << BOLDCYAN << n_clients << RESET " clients created." << std::endl;

}

pid_t create_single_process(const std::string& path, const std::string& clas, char **argv)
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
            if (execv(path.c_str(), argv) == -1) {
                fprintf(stderr, "[MANAGER] Error using execl() in %s process: %s.\n",
                        clas.c_str(), strerror(errno));
                terminate_processes();
                free_resources();
                std::exit(EXIT_FAILURE);
            }
    }
    return pid;
}

void get_str_process_info(enum ProcessClass_t clas, std::string *path, std::string *str_process_class)
{
    switch (clas) {
        case ILIMITED_PREMIUM_CLIENT:
            *path = CLIENT_PREMIUM_PATH;
            *str_process_class =  ILIMITED_PREMIUM_CATEGORY;
        break;
        case PREMIUM_CLIENT:
            *path = CLIENT_PREMIUM_LIMIT_PATH;
            *str_process_class =  PREMIUM_CATEGORY;
        break;
        case NORMAL_CLIENT:
            *path = CLIENT_NORMAL_PATH;
            *str_process_class =  NORMAL_CATEGORY;
        break;
        case PAYMENT_SYSTEM:
            *path = PAYMENT_SYSTEM_PATH;
            *str_process_class = PAYMENT_SYSTEM_CLASS;
        break;
    }
}


/********************************* SEMAPHORES AND SHARED MEMORY MANAGEMENT ********************************************/

void create_shm_segments(int *shm_payment, struct T_Payment **p_payment, int *shm_client, struct TRequest_t **p_request)
{
    /* Create and initialize shared memory segments */
    *shm_payment = shm_open(SHM_PAYMENT, O_CREAT | O_RDWR, 0644);
    ftruncate(*shm_payment, sizeof(struct TPayment));
    *p_payment = static_cast<T_Payment *>(mmap(NULL, sizeof(struct TPayment),
            PROT_READ | PROT_WRITE, MAP_SHARED, *shm_payment, 0));

    *shm_client = shm_open(SHM_CLIENT, O_CREAT | O_RDWR, 0644);
    ftruncate(*shm_client, sizeof(struct TRequest_t));
    *p_request = static_cast<TRequest_t *>(mmap(NULL, sizeof(struct TRequest_t),
                                                PROT_READ | PROT_WRITE, MAP_SHARED, *shm_client, 0));
}
void create_sems(sem_t **sem_balance_ready, sem_t **sem_balance_charge, sem_t **sem_request_ready,
                 sem_t **sem_stored_request, sem_t **sem_replic_finish)
{
    *sem_balance_ready = create_semaphore(SEM_BALANCE_READY,0);
    *sem_balance_charge = create_semaphore(SEM_BALANCE_CHARGE,0);
    *sem_request_ready = create_semaphore(SEM_REQUEST_READY,0);
    *sem_stored_request = create_semaphore(SEM_STORED_REQUEST,0);
    *sem_replic_finish = create_semaphore(SEM_REPLIC_FINISH,0);

    create_semaphore(SEM_MUTEX,1);
}
void close_shared_memory_segments(int shm_payment, int shm_client)
{
    close(shm_payment);
    close(shm_client);
}

/*************************************** MANAGEMENT PROCESSES TERMINATION *******************************/

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
        /* Child process alive*/
        std::cout << "[BUSCADOR] Terminating " << v_clients[i].str_process_class <<
        " process [" << v_clients[i].pid << "]..." << std::endl;
        if (kill(v_clients[i].pid, SIGINT) == -1) {
            fprintf(stderr, "[BUSCADOR] Error using kill() on process %d: %s.\n",
                    v_clients[i].pid, strerror(errno));
        }

    }
}
void free_resources()
{
    std::cout << "\n----- [BUSCADOR] Free resources ----- " << std::endl;

    v_clients = std::vector<struct TProcess_t>();
    request_vector = std::vector<struct TRequest_t>();

    /* Semaphores */
    remove_semaphore(SEM_BALANCE_READY);
    remove_semaphore(SEM_BALANCE_CHARGE);
    remove_semaphore(SEM_REQUEST_READY);
    remove_semaphore(SEM_STORED_REQUEST);
    remove_semaphore(SEM_MUTEX);

    /* Shared memory segments*/
    shm_unlink(SHM_PAYMENT);
    shm_unlink(SHM_CLIENT);
}

/*************************************** SIGNALS MANAGEMENT *************************/

void install_signal_handler(int signal_number){
    if (signal(signal_number, signal_handler) == SIG_ERR) {
        fprintf(stderr, "%s[MANAGER] Error installing signal handler: %s%s.\n",RED, strerror(errno),RESET);
        exit(EXIT_FAILURE);
    }
}

void signal_handler(int signal){
    std::cout << "[BUSCADOR] Exiting..." << std::endl;
    terminate_processes();
    free_resources();
    std::exit(EXIT_SUCCESS);
}


