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
#include <stdlib.h>
#include <time.h>

#include "Text.h"
#include "Coincidence_Format.h"
#include "colors.h"
#include "definitions.h"
#include "mycmp.h"

class Client_Management{
private:
    unsigned thread_id;
    std::string pattern;
    std::vector<Text> v_txts;
    std::vector<std::thread> v_hilos;
    struct TRequest_t request_client;

public:
    std::priority_queue<Coincidence_Format, std::vector<Coincidence_Format>,
            myComp> coincidences;
    Client_Management(unsigned id);
    ~Client_Management();
    void add_coincidence(Coincidence_Format coincidence);
    void start_finding();
    void find(int begin, int end, std::string categoria,std::string text_name, Text txt);
    std::string getCoincidences();
    unsigned getthreadid();
    int get_number_coindicences();

    [[noreturn]] void wait_for_request();
};




#endif //SSOO_P3_CLIENT_MANAGEMENT_H
