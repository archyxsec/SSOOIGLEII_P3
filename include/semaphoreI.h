/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : semaphoreI.h
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 21/04/2021
*
* Purpose : declaración de funciones de semaphoreI
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 25/04/2021    Tomás           1        Incluidas librerias de C
* 25/04/2021    Tomás           2        cambiado const char por std::string
* 13/05/2021    Tomás           2        Cambiado parametros de funciones por const std::string&
*
|********************************************************/

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

sem_t *create_semaphore (const std::string& name, unsigned int value);
sem_t *get_semaphore    (const std::string& name);
void remove_semaphore   (const std::string& name);
void signal_semaphore   (sem_t *sem);
void wait_semaphore     (sem_t *sem);

#endif //SSOO_P3_SEMAPHOREI_H
