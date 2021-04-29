//
// Created by TOMIC on 28/04/2021.
//

#ifndef SSOO_P3_CLIENT_H
#define SSOO_P3_CLIENT_H

#include "Text.h"


class Client{
protected:
    int _id;
    std::string _word;
    std::string _category;
    std::vector<Text> _text_vector;
    Client(int id, std::string word, std::string categoria);
    ~Client();
    void request_search();

};

#endif //SSOO_P3_CLIENT_H
