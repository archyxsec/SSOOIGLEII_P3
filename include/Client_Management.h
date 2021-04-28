//
// Created by TOMIC on 26/04/2021.
//

#ifndef SSOO_P3_CLIENT_MANAGEMENT_H
#define SSOO_P3_CLIENT_MANAGEMENT_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <exception>
#include <thread>

#include "Text.h"
#include "Coincidence_Format.h"
#include "colors.h"
#include "definitions.h"

class Client_Management{
private:
    unsigned thread_id;
    unsigned begin;
    unsigned end;
    std::string pattern;
    Text txt;
    std::vector<Coincidence_Format> coincidences;
    std::vector<std::thread> v_hilos;
    struct TRequest_t request_client;

public:

    Client_Management(unsigned id, unsigned _begin, unsigned _end, Text _txt, std::string _pattern);
    ~Client_Management();
    void start_finding();
    void add_coincidence(Coincidence_Format coincidence);
    std::string getCoincidences() const;
    unsigned getthreadid();
    int get_number_coindicences();
};


#endif //SSOO_P3_CLIENT_MANAGEMENT_H
