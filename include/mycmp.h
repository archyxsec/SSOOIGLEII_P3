/******************************************************
* Project : Práctica 2 de Sistemas Operativos II
*
* Program name : mycmp.h
*
* Author : Tomás Jesús García López
*
* Date created : 13/04/2021
*
* Purpose : Definición de clase Comparador para la cola con prioridad del Main
*
*
* Revision History:
*
* Date          Author          Ref      Revision
*
*
|********************************************************/

#ifndef PRACTICA2_MYCMP_H
#define PRACTICA2_MYCMP_H

#include "Coincidence_Format.h"

class myComp {
public:
    bool operator()(Thread_Management& thrm1, Thread_Management& thrm2);
};

#endif //PRACTICA2_MYCMP_H