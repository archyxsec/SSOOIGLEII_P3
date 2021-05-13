/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : mycmp.cpp
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 29/04/2021
*
* Purpose : clase mycmp encargada de establecer la prioridad en nuestra cola de coincidencias
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 05/05/2021    Tomás           1        Solucionado line_number, antes era un metodo get, ahora es directamente el atributo que es público
*
|********************************************************/

#include "../include/mycmp.h"

bool myComp::operator()(Coincidence_Format &cf1, Coincidence_Format &cf2) {
    return cf1.line_number > cf2.line_number;
}