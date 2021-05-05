/******************************************************
* Project : Práctica 2 de Sistemas Operativos II
*
* Program name : mycmp.h
*
* Author : Tomás Jesús García López
*
* Date created : 13/04/2021
*
* Purpose : clase Comparador para la cola con prioridad del Main
*
*
* Revision History:
*
* Date          Author          Ref      Revision
*
*
|********************************************************/

#include "../include/mycmp.h"

bool myComp::operator()(Coincidence_Format &cf1, Coincidence_Format &cf2) {
    return cf1.line_number > cf2.line_number;
}