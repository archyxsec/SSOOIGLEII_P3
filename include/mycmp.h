/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : mycmp.h
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 29/04/2021
*
* Purpose : Definición de clase myComp
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 05/05/2021    Tomás           1        Cambiado Thread_Manager de mi anterior práctica por Coincidence_Format
*
|********************************************************/

#ifndef PRACTICA2_MYCMP_H
#define PRACTICA2_MYCMP_H

#include "Coincidence_Format.h"

class myComp {
public:
    bool operator()(Coincidence_Format &cf1, Coincidence_Format &cf2);
};

#endif //PRACTICA2_MYCMP_H