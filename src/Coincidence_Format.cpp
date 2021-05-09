//
// Created by TOMIC on 26/04/2021.
//

#include "../include/Coincidence_Format.h"


Coincidence_Format::Coincidence_Format(unsigned l_number, std::string patt, std::string text_name)
{
    this->line_number = l_number;
    this->pattern = patt;
    this->_text_name = text_name;
}

void Coincidence_Format::set_coincidence(std::string previus_word, std::string word, std::string post_word)
{
    this->coincidence.previus_word = previus_word;
    this->coincidence.word = word;
    this->coincidence.post_word = post_word;
}
std::string Coincidence_Format::get_file_name() const {
    return this->_text_name;
}
