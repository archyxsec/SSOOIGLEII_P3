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
#include <unistd.h>
#include <time.h>
#include <mutex.h>

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
    std::vector<std::thread> v_threads;
    struct TRequest_t request_client;
    std::mutex mutex;

public:
    std::priority_queue<Coincidence_Format, std::vector<Coincidence_Format>,
            myComp> coincidences;
    Client_Management(unsigned id);
    ~Client_Management();
    void add_coincidence(Coincidence_Format coincidence);
    void start_finding(int id, std::string category, Text txt, int begin, int end, std::string pattern);
    //void find(int begin, int end, std::string categoria,std::string text_name, Text txt);
    std::string getCoincidences(int fd_descriptor);
    unsigned getthreadid();
    int get_number_coindicences();
    void free_resources();
    [[noreturn]] void wait_for_request();
};




#endif //SSOO_P3_CLIENT_MANAGEMENT_H
