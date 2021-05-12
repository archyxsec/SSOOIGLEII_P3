#include "../include/Client_Premium.h"

#define LECTURA 0
#define ESCRITURA 1

//Client_Premium::Client_Premium(int id, std::string word, std::string category) : Client(id, word, category){}
char pipename[MAX_BUFFER_TEXT];

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
    char Buffer[MAX_BUFFER_TEXT];
    *v_texts_name = (char*)malloc(MAX_BUFFER_TEXT*sizeof(char));
    if(argc < 3){
        fprintf(stderr,"[CLIENT_PREMIUM %i] Error, use: ./exec/Client_Premium <pattern> [<texts>]",getpid());
        std::exit(EXIT_FAILURE);
    }
    *word = argv[1];
    strcpy(*v_texts_name, argv[2]);
    for(int i=3; i<argc; i++){
        if(fopen(argv[i],"r") != NULL){
            sprintf(Buffer,"-%s",argv[i]);
            strncat(*v_texts_name,Buffer, sizeof(Buffer));
        }
    }
}
void signal_handler(int signal){
    std::cout << "[CLIENT_PREMIUM] Exiting..." << std::endl;
    free_resources();
}

void install_signal_handler() {
    if ((signal(SIGUSR1, signal_handler)) == SIG_ERR) {
        fprintf(stderr, "[CLIENT_PREMIUM] Error installing signal handler: %s.\n", strerror(errno));
        std::exit(EXIT_FAILURE);
    }
}

void free_resources(){

    shm_unlink(SHM_CLIENT);
    unlink(pipename);

}
int main(int argc, char **argv){
    sem_t *p_sem_request_ready, *p_sem_stored_request;
    struct TRequest_t *client_premium;
    char *v_texts_name;
    char *word;
    int shm_client;
    char coincidences[MAX_BUFFER_TEXT];
    int mypipe;


    install_signal_handler();
    parse_argv(argc, argv, reinterpret_cast<char **>(&word), reinterpret_cast<char **>(&v_texts_name));

    /*Create the pipe*/
    sprintf(pipename,"/tmp/client%d",getpid());
    mknod(pipename,S_IFIFO | S_IRUSR | S_IWUSR,0);

    get_shm_segments(&shm_client, &client_premium);
    get_sems(&p_sem_request_ready, &p_sem_stored_request);
    std::cout << "[CLIENT_PREMIUM " << getpid() << "] I'm gonna send the request" << std::endl;
    wait_semaphore(p_sem_request_ready);

    client_premium->client_pid = getpid();
    strcpy(client_premium->category,ILIMITED_PREMIUM_CATEGORY);
    strcpy(client_premium->word,word);
    client_premium->initial_balance = -1;
    strcpy(client_premium->pipename,pipename);
    strcpy(client_premium->v_texts,v_texts_name);

    signal_semaphore(p_sem_stored_request);
    mypipe = open(pipename, O_RDONLY);

    while(read(mypipe,coincidences,MAX_BUFFER_TEXT) > 0) std::cout << coincidences;
    std::cout << "[CLIENT_PREMIUM " << getpid() << "] Im Finnish!" << std::endl;
    close(mypipe);
    pause();
    free_resources();
    return EXIT_SUCCESS;
}
