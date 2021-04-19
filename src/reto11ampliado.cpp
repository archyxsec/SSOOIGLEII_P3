//
// Created by TOMIC on 19/04/2021.
//

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
#include <condition_variable>

#define N 10

int turno;
std::mutex mut;
std::mutex sincroprincipal;
std::unique_lock<std::mutex> ul(sincroprincipal);
std::condition_variable cv;

void run(int i)
{
    cv.wait(ul,[i]{return turno==i;});
    std::cout << "Hilo " << i << " Es mi turno y turno vale" << turno << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    mut.unlock();
}

int main(int argc, char **argv){
    std::vector<std::thread> vhilos;
    mut.lock();

    srand(time(NULL));
    for(int i = 0; i < N;i++){
        vhilos.push_back(std::thread(run,i));
    }

    for(int i = 0; i < N; i++){
        //turno = 1 + rand() % (N);
        turno = i;
        cv.notify_all();
        sincroprincipal.unlock();
        mut.lock();
    }
    //return turno == id;
    std::for_each(vhilos.begin(), vhilos.end(), [](std::thread& t) { t.join(); });
    std::cout << "Hilo principal, han terminado todos los hilos " << std::endl;

}

