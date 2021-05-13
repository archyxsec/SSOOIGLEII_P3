/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : Coincidence_Format.cpp
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 26/04/2021
*
* Purpose : Constructor , getter y Método de la clase Coincidence_Format encargado de añadir coincidencias
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 29/04/2021    Tomás           1        Cambio char* por std::string
* 10/05/2021    Tomás           2        get_file_name establecido como const
*
|********************************************************/

#include "../include/Coincidence_Format.h"

Coincidence_Format::Coincidence_Format(unsigned l_number, std::string patt, std::string text_name)
{
    this->line_number = l_number;
    this->pattern = std::move(patt);
    this->_text_name = std::move(text_name);
}

void Coincidence_Format::set_coincidence(std::string previus_word, std::string word, std::string post_word)
{
    this->coincidence.previus_word = std::move(previus_word);
    this->coincidence.word = std::move(word);
    this->coincidence.post_word = std::move(post_word);
}

std::string Coincidence_Format::get_file_name() const {
    return this->_text_name;
}
