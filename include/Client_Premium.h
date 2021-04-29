//
// Created by TOMIC on 28/04/2021.
//

#ifndef SSOO_P3_CLIENT_PREMIUM_H
#define SSOO_P3_CLIENT_PREMIUM_H

#include "Client_Normal.h"

class Client_Premium : protected Client{
public:
    Client_Premium(int id, std::string word, std::string categoria);
};

#endif //SSOO_P3_CLIENT_PREMIUM_H
