/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : Client_Premium_Limit.cpp
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 28/04/2021
*
* Purpose : funciones de Client_Premium_Limit
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 05/05/2021    Sergio          1        Creado main y funciones de manejo de semáforos y memoria compartida
* 10/05/2021    Sergio          2        Implementadas resto de funciones
* 13/05/2021    Sergio          3        Eliminadas funciones comunes a todos los clientes,estan en Clients_Common
*
*
|********************************************************/

#include "../include/Client_Premium_Limit.h"

int main(int argc, char **argv){
    sem_t *p_sem_request_ready, *p_sem_stored_request;
    struct TRequest_t *client_premium;
    char *v_texts_name;
    char *word;
    int shm_client;
    char coincidences[MAX_BUFFER_TEXT];
    int n_credits;
    int mypipe;

    install_signal_handler();
    parse_argv(argc, argv, &word, &v_texts_name, &n_credits);

    /*Create the pipe*/
    sprintf(pipename,"/tmp/client%d",getpid());
    mknod(pipename,S_IFIFO | S_IRUSR | S_IWUSR,0); // create the pipe file

    get_shm_segments(&shm_client, &client_premium);
    get_sems(&p_sem_request_ready, &p_sem_stored_request);
    std::cout << "[CLIENT_PREMIUM_LIMIT " << getpid() << "] I'm gonna send the request" << std::endl;
    /*Rendezvouz patron to send the request*/
    wait_semaphore(p_sem_request_ready);

    client_premium->client_pid = getpid();
    strcpy(client_premium->category,PREMIUM_CATEGORY);
    strcpy(client_premium->word,word);
    client_premium->initial_balance = n_credits;
    strcpy(client_premium->pipename,pipename);
    strcpy(client_premium->v_texts,v_texts_name);

    signal_semaphore(p_sem_stored_request);
    mypipe = open(pipename, O_RDONLY); // open the pipe

    /*Wait and print the results*/
    while(read(mypipe,coincidences,MAX_BUFFER_TEXT) > 0) std::cout << coincidences;
    std::cout << "[CLIENT_PREMIUM_LIMIT " << getpid() << "] Im Finnish!" << std::endl;
    close(mypipe);
    pause(); // wait for Buscador signal to termination
    free_resources();
    return EXIT_SUCCESS;
}

/********************************** ARGUMENT MANAGEMENT *************************************************/

void parse_argv(int argc, char **argv, char **word, char **v_texts_name, int *n_credits)
{
    char Buffer[MAX_BUFFER_TEXT];
    *v_texts_name = (char*)malloc(MAX_BUFFER_TEXT*sizeof(char));
    if(argc < 4){
        fprintf(stderr,"%s[CLIENT_PREMIUM_LIMIT %i] Error%s, use: ./exec/Client_Premium_Limit <pattern> <n_credits> [<texts>]",RED,getpid(),RESET);
        std::exit(EXIT_FAILURE);
    }
    *word = argv[1];
    *n_credits = atoi(argv[2]);
    strcpy(*v_texts_name, argv[3]);
    for(int i=4; i<argc; i++){
        if(fopen(argv[i],"r") != nullptr){
            sprintf(Buffer,"-%s",argv[i]);
            strncat(*v_texts_name,Buffer, sizeof(Buffer));
        }
    }
}

/******************* PROCESS COMUNICATION ******************/

void install_signal_handler() {
    if ((signal(SIGINT, signal_handler)) == SIG_ERR) {
        fprintf(stderr, "%s[CLIENT_PREMIUM_LIMIT] Error installing signal handler: %s%s.\n",RED, strerror(errno),RESET);
        std::exit(EXIT_FAILURE);
    }
}

void signal_handler(int signal){
    std::cout << "[CLIENT_PREMIUM_LIMIT] Exiting..." << std::endl;
    free_resources();
}

/********** FREE RESOURCES *******/

void free_resources(){
    shm_unlink(SHM_CLIENT);
    unlink(pipename);
}
