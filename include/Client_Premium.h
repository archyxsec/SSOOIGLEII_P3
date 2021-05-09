//
// Created by TOMIC on 28/04/2021.
//

#ifndef SSOO_P3_CLIENT_PREMIUM_H
#define SSOO_P3_CLIENT_PREMIUM_H

#define _POSIX_SOURCE // for pause(2)
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include "definitions.h"
#include "Client_Normal.h"
#include "semaphoreI.h"

void get_sems(sem_t **p_sem_request_ready, sem_t **p_sem_stored_request);
void get_shm_segments(int *shm_client, struct TRequest_t **p_request);
void parse_argv(int argc, char **argv, char **word, char **v_texts_name);

/*Process Communication*/
//void install_signal_handler();

//void signal_handler(int signal);
void free_resources();

#endif //SSOO_P3_CLIENT_PREMIUM_H
