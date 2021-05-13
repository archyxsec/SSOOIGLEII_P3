/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : Buscador.h
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 21/04/2021
*
* Purpose : declaración de funciones del Buscador
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 25/04/2021    Tomás           1        Declaración de funciones
* 25/04/2021    Tomás           2        incompatibilidad entre std::string y memoria compartida en C.
* 05/05/2021    Tomás           3        Variables de condición y vector de peticiones. También función wait_requests
*                                        correspondiente a un hilo
* 07/05/2021    Tomás           4        Declaraciones de wait_requests(), manage_queue() y create_client_management()
* 07/05/2021    Tomás           5        añadido [[noreturn]] y install_signal_handler y signal_handler
* 10/05/2021    Tomás           6        cambiados parametros de create_client_management
* 12/05/2021    Tomás           7        Variables atómicas correspondientes a el número de replicas y clientes atendidos. Organización del código
* 13/05/2021    Tomás           8        Variables total_clients_attends y total_clients_requests añadidas a la solición
*
|********************************************************/

#ifndef SSOO_P3_BUSCADOR_H
#define SSOO_P3_BUSCADOR_H

#include <dirent.h>
#include <atomic>

#include "semaphoreI.h"
#include "definitions.h" // Libraries and defines
#include "colors.h"

int g_nProcesses = N_CLIENTS + N_PAYMENT_SYSTEM;
std::mutex queue_semaphore_management; // Mutex semaphore for guaranted exclusive access to critical seccion
std::mutex mutex; // Queue access
std::condition_variable extract_request_condition;
std::vector<struct TProcess_t> v_clients; //vector of clients processes
std::vector<struct TRequest_t> request_vector(0);
std::atomic<int> n_replics(0); //atomic n_replics for client_management processes control.

/*Payment_system PID*/
pid_t payment_process;
int total_clients_attends = 0;
int total_clients_requests = 0;

/******************************** CLIENT REQUESTS MANAGEMENT *****************************************************/
[[noreturn]] void manage_clients_management_termination(sem_t *sem_replic_finish);
[[noreturn]] void wait_requests(sem_t *sem_request_ready, sem_t *sem_stored_request,struct TRequest_t *request);
[[noreturn]] void manage_queue();
void create_client_management(char *v_texts, char *word,
                              char *pipename, int initial_balance, char *category, int client_pid);
int get_text_len(char *v_texts);

/**************************** PROCESSES MANAGEMENT **********************************************/
void create_payment_system(enum ProcessClass_t clas);
void create_aleatory_clients(int n_clients);
pid_t create_single_process(const std::string& path, const std::string& clas, char **argv);
void get_str_process_info(enum ProcessClass_t clas, std::string *path, std::string *str_process_class);

/********************************* SEMAPHORES AND SHARED MEMORY MANAGEMENT ********************************************/
void create_shm_segments(int *shm_payment, struct T_Payment **p_payment,
        int *shm_client, struct TRequest_t **p_request);
void create_sems(sem_t **sem_balance_ready, sem_t **sem_balance_charge, sem_t **sem_request_ready,
                 sem_t **sem_stored_request, sem_t **sem_replic_finish);
void close_shared_memory_segments(int shm_payment, int shm_client);

/*************************************** MANAGEMENT PROCESSES TERMINATION *******************************/
void terminate_processes();
void free_resources();

/*************************************** SIGNALS MANAGEMENT *************************/
void install_signal_handler(int signal_number);
void signal_handler(int signal);

#endif //SSOO_P3_BUSCADOR_H
