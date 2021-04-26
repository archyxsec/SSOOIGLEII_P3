//
// Created by TOMIC on 26/04/2021.
//

#include "../include/Coincidence_Format.h"


Coincidence_Format::Coincidence_Format(unsigned l_number, std::string patt)
{
    this->line_number = l_number;
    this->pattern = patt;
}

void Coincidence_Format::set_coincidence(std::string previus_word, std::string word, std::string post_word)
{
    this->coincidence.previus_word = previus_word;
    this->coincidence.word = word;
    this->coincidence.post_word = post_word;
}
