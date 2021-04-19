//
// Created by TOMIC on 19/04/2021.
//

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

#define N 10

int turno;
std::mutex sincroprincipal;

void run(unsigned i)
{
    while(turno != i);
    std::cout << "Hilo " << i << " Es mi turno" << std::endl;
    sincroprincipal.unlock();
}

int main(int argc, char **argv){
    std::vector<std::thread> vhilos;
    srand(time(NULL));
    for(unsigned i = 0; i < N;i++){
        vhilos.push_back(std::thread(run,i));
    }
    for(unsigned i = 0; i < N; i++){
        //turno = 1 + rand() % (N);
        turno = i;
        sincroprincipal.lock();
    }
    std::for_each(vhilos.begin(), vhilos.end(), [](std::thread& t) { t.join(); });
    std::cout << "Hilo principal, han terminado todos los hilos " << std::endl;

}
