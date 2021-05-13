/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : Coincidence_Format.h
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 26/04/2021
*
* Purpose : definición de estructura pattern y clase Coincidence_Format
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 29/04/2021    Tomás           1        Cambio char* por std::string
*
|********************************************************/

#ifndef SSOO_P3_COINCIDENCE_FORMAT_H
#define SSOO_P3_COINCIDENCE_FORMAT_H

#include <iostream>
#include <string>
#include <vector>

/* Pattern struct for build the coincidence message */
struct pattern{
    std::string previus_word;
    std::string word;
    std::string post_word;
};

class Coincidence_Format{
private:
    std::string pattern;
    std::string _text_name;
public:
    Coincidence_Format(unsigned l_number, std::string patt, std::string text_name);
    void set_coincidence(std::string previus_word, std::string word, std::string post_word);
    struct pattern coincidence;
    std::string get_file_name() const;
    unsigned line_number;
};

#endif //SSOO_P3_COINCIDENCE_FORMAT_H
