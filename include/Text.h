//
// Created by TOMIC on 26/04/2021.
//

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
    std::string getlinetext(int n_line);
public:
    unsigned n_lines;
    std::string file_name;
    Text(std::string filename);
    Text(){};
    unsigned get_number_of_lines();
    std::vector<std::string> getlinevector(int n_line);
    std::string wordwrapper(std::string word);
    bool extraer(char character);
};

#endif //SSOO_P3_TEXT_H
