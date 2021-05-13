/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : Text.h
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 26/04/2021
*
* Purpose : definición de clase Text
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 03/05/2021    Tomás           1        Cambiado file_name a public para ser accesible desde otros archivos
* 13/05/2021    Tomás           2        Cambiado algunás funciones, puestas estáticas
*
|********************************************************/


#ifndef SSOO_P3_TEXT_H
#define SSOO_P3_TEXT_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstring>
#include <cctype>

class Text{
private:
    std::string getlinetext(int n_line) const;
public:
    unsigned n_lines;
    std::string file_name;
    Text(std::string filename);
    Text(){};
    unsigned get_number_of_lines() const;
    std::vector<std::string> getlinevector(int n_line);
    static std::string wordwrapper(std::string word);
    static bool extraer(char character);
};

#endif //SSOO_P3_TEXT_H
