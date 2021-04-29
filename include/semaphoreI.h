//
// Created by TOMIC on 21/04/2021.
//

#ifndef SSOO_P3_SEMAPHOREI_H
#define SSOO_P3_SEMAPHOREI_H

#include <semaphore.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <string>

sem_t *create_semaphore (std::string name, unsigned int value);
sem_t *get_semaphore    (std::string name);
void remove_semaphore   (std::string name);
void signal_semaphore   (sem_t *sem);
void wait_semaphore     (sem_t *sem);

#endif //SSOO_P3_SEMAPHOREI_H
