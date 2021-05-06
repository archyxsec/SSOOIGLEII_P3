#include "../include/Client_Premium.h"

#define LECTURA 0
#define ESCRITURA 1

//Client_Premium::Client_Premium(int id, std::string word, std::string category) : Client(id, word, category){}
int p[2];

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
void parse_argv(int argc, char **argv, char **word, char **v_texts_name)
{
    if(argc < 3){
        fprintf(stderr,"[CLIENT_PREMIUM %i] Error, use: ./exec/Client_Premium <pattern> [<texts>]",getpid());
        std::exit(EXIT_FAILURE);
    }
    *word = argv[1];
    int j = 0;
    for(int i=2; i<argc; i++){
        if(fopen(argv[i],"r") != NULL){
            strncat(*v_texts_name,argv[i], sizeof(argv[i]));
        }
    }
}
void install_signal_handler() {
    if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
        fprintf(stderr, "[MANAGER] Error installing signal handler: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}
void signal_handler(int signal){
    char buffer[BUFFER_RESULTS];
    std::cout << "[CLIENT_PREMIUM " << getpid() << "] Client Manager send me the results!" << std::endl;
    read(p[LECTURA],&buffer,BUFFER_RESULTS);
    printf("%s",buffer);
    std::cout << "[CLIENT_PREMIUM " << getpid() << "] Im Finnish!" << std::endl;
}
int main(int argc, char **argv){
    sem_t *p_sem_request_ready, *p_sem_stored_request;
    struct TRequest_t *client_premium;
    struct TRequest_t *myrequest;
    char v_texts_name[MAX_BUFFER_TEXT];
    char word[MAX_BUFFER_TEXT];
    char *category[MAX_BUFFER_TEXT];
    int shm_client;

    install_signal_handler();
    std::cout << "Antes del parse" << std::endl;
    //parse_argv(argc, argv, reinterpret_cast<char **>(&word), reinterpret_cast<char **>(&v_texts_name));
    if(argc < 3){
        fprintf(stderr,"[CLIENT_PREMIUM %i] Error, use: ./exec/Client_Premium <pattern> [<texts>]",getpid());
        std::exit(EXIT_FAILURE);
    }
    strcpy(word,argv[1]);
    int j = 0;
    for(int i=2; i<argc; i++){
        if(fopen(argv[i],"r") != NULL){
            strncat(v_texts_name,argv[i], sizeof(argv[i]));
        }
    }

    pipe(p);
    std::cout << word << std::endl;
    close(p[ESCRITURA]);
    std::cout <<v_texts_name << std::endl;

    get_shm_segments(&shm_client, &client_premium);
    get_sems(&p_sem_request_ready, &p_sem_stored_request);

    std::cout << "[CLIENT_PREMIUM " << getpid() << "] I'm gonna send the request" << std::endl;
    wait_semaphore(p_sem_request_ready);
    client_premium->client_pid = getpid();
    strcpy(client_premium->category,ILIMITED_PREMIUM_CATEGORY);
    strcpy(client_premium->word,word);
    client_premium->initial_balance = -1;
    client_premium->fd_descriptor = p[ESCRITURA];
    strcpy(client_premium->v_texts,v_texts_name);
    std::cout << "Antes del signal sem stored" << std::endl;
    signal_semaphore(p_sem_stored_request);
    pause();
    return EXIT_SUCCESS;
}
