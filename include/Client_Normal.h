//
// Created by TOMIC on 28/04/2021.
//

#ifndef SSOO_P3_CLIENT_NORMAL_H
#define SSOO_P3_CLIENT_NORMAL_H

#include <iostream>
#include <string>
#include <vector>
#include "Client.h"

class Client_Normal : protected Client{
private:
    int n_searches;

public:
    Client_Normal(int id, std::string word, std::string categoria, int n_searches);
    ~Client_Normal();
    int get_n_searches();

};

#endif //SSOO_P3_CLIENT_NORMAL_H
