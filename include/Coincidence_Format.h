//
// Created by TOMIC on 26/04/2021.
//

#ifndef SSOO_P3_COINCIDENCE_FORMAT_H
#define SSOO_P3_COINCIDENCE_FORMAT_H

#include <iostream>
#include <string>
#include <vector>

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
    unsigned line_number;
};

#endif //SSOO_P3_COINCIDENCE_FORMAT_H
