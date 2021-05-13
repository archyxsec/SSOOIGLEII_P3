/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : Client_Premium_Limit.h
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 28/04/2021
*
* Purpose : declaración de funciones de Client_Premium_Limit
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 05/05/2021    Sergio          1        Librerias, funciones y métodos de clases
* 07/05/2021    Sergio          2        Borrado clase, declaración de funciones.
* 13/05/2021    Sergio          3        Eliminadas funciones comunes a todos los clientes,estan en Clients_Common
*
|********************************************************/

#ifndef SSOO_P3_CLIENT_PREMIUM_LIMIT_H
#define SSOO_P3_CLIENT_PREMIUM_LIMIT_H

#include "Clients_Common.h"

char pipename[MAX_BUFFER_TEXT];

/********************************** ARGUMENT MANAGEMENT *************************************************/
void parse_argv(int argc, char **argv, char **word, char **v_texts_name, int *n_credits);

/******************* PROCESS COMUNICATION ******************/
void install_signal_handler();
void signal_handler(int signal);

/********** FREE RESOURCES *******/
void free_resources();

#endif //SSOO_P3_CLIENT_PREMIUM_LIMIT_H
