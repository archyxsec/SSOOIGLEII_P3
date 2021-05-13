/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : Client_Premium.cpp
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 28/04/2021
*
* Purpose : funciones de Client_Premium
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 05/05/2021    Sergio          1        Creado main y funciones de manejo de semáforos y memoria compartida
* 07/05/2021    Sergio          2        Implementando funciones
* 07/05/2021    Sergio          3        Cambiando funciones para adaptar los args
* 10/05/2021    Sergio          4        free_resources y parse_argv adaptados
* 10/05/2021    Sergio          5        Implementadas resto de funciones
* 13/05/2021    Sergio          6        Eliminadas funciones comunes a todos los clientes,estan en Clients_Common
*
*
|********************************************************/

#include "../include/Client_Premium.h"

int main(int argc, char **argv){
    sem_t *p_sem_request_ready, *p_sem_stored_request;
    struct TRequest_t *client_premium;
    char *v_texts_name;
    char *word;
    int shm_client;
    char coincidences[MAX_BUFFER_TEXT];
    int mypipe;

    install_signal_handler();
    parse_argv(argc, argv, &word, &v_texts_name);

    /*Create the pipe*/
    sprintf(pipename,"/tmp/client%d",getpid());
    mknod(pipename,S_IFIFO | S_IRUSR | S_IWUSR,0); // create the pipe file

    get_shm_segments(&shm_client, &client_premium);
    get_sems(&p_sem_request_ready, &p_sem_stored_request);
    std::cout << "[CLIENT_PREMIUM " << getpid() << "] I'm gonna send the request" << std::endl;
    /*Rendezvouz patron to send the request*/
    wait_semaphore(p_sem_request_ready);

    client_premium->client_pid = getpid();
    strcpy(client_premium->category,ILIMITED_PREMIUM_CATEGORY);
    strcpy(client_premium->word,word);
    client_premium->initial_balance = -1;
    strcpy(client_premium->pipename,pipename);
    strcpy(client_premium->v_texts,v_texts_name);

    signal_semaphore(p_sem_stored_request);
    mypipe = open(pipename, O_RDONLY); // open the pipe

    /*Wait and print the results*/
    while(read(mypipe,coincidences,MAX_BUFFER_TEXT) > 0) std::cout << coincidences;
    std::cout << "[CLIENT_PREMIUM " << getpid() << "] Im Finnish!" << std::endl;
    close(mypipe);
    pause(); // wait for Buscador signal to termination
    free_resources();
    return EXIT_SUCCESS;
}

/********************************** ARGUMENT MANAGEMENT *************************************************/

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
        if(fopen(argv[i],"r") != nullptr){
            sprintf(Buffer,"-%s",argv[i]);
            strncat(*v_texts_name,Buffer, sizeof(Buffer));
        }
    }
}

/******************* PROCESS COMUNICATION ******************/

void signal_handler(int signal){
    std::cout << "[CLIENT_PREMIUM " << getpid() << "] Exiting..." << std::endl;
    free_resources();
}

void install_signal_handler() {
    if ((signal(SIGINT, signal_handler)) == SIG_ERR) {
        fprintf(stderr, "%s[CLIENT_PREMIUM %i] Error installing signal handler: %s%s.\n",RED,getpid() ,strerror(errno),RESET);
        std::exit(EXIT_FAILURE);
    }
}

/********** FREE RESOURCES *******/

void free_resources(){
    shm_unlink(SHM_CLIENT);
    unlink(pipename);
}
