//
// Created by TOMIC on 26/04/2021.
//

#include "../include/Client_Management.h"

Client_Management::Client_Management(unsigned id)
{
   this->thread_id = id;
   this->wait_for_request();
}

[[noreturn]] void Client_Management::wait_for_request()
{
    std::string category;
    std::string pattern;
    int initial_balance;
    int fd_descriptor;

    int random_number;
    bool vip;
    bool choose = false;

    srand(time(NULL));

    for(;;)
    {
        std::unique_lock<std::mutex> ul(queue_semaphore_management);

        extract_request_condition.wait(ul,[]{
            return request_queue.size() <= N_REPLICS;
        });

        /*Randomize number in order to choose 80% request for premium clients, and 20% normal client*/
        random_number = 1 + rand() % (10);
        vip = (random_number <= 8 ? true : false);
        int i;

        for(i=0;i<request_queue.size();i++)
        {
            if(vip && (request_queue[i].category == PREMIUM_CATEGORY ||
                request_queue[i].category == ILIMITED_PREMIUM_CATEGORY)) choose = true;
            else if(request_queue[i].category == NORMAL_CATEGORY) choose = true;

            if(choose){
                category = request_queue[i].category;
                pattern = request_queue[i].pattern;
                initial_balance = request_queue[i].initial_balance;
                v_txts = request_queue[i].v_texts;
                break;
            }
        }
        v_txts.erase(v_txts.begin() + i);
        extract_request_condition.notify_one();
        ul.unlock(); /*Unlock the semaphore*/

        /*Next, We divide the work by threads*/

    }
}

Client_Management::~Client_Management()
{
    /*Free vectors*/
    this->v_hilos = std::vector<std::thread>();
    this->coincidences = std::priority_queue<Coincidence_Format, std::vector<Coincidence_Format>,
            myComp>();
    this->v_txts = std::vector<Text>();
}

void Client_Management::add_coincidence(Coincidence_Format coincidence) {
    this->coincidences.push(coincidence);
}

unsigned Client_Management::getthreadid()
{
    return this->thread_id;
}
int Client_Management::get_number_coindicences()
{
    return this->coincidences.size();
}
std::string Client_Management::getCoincidences(){
    std::string format_line = "";
    std::string commun_start_line;
    //commun_start_line = "[Hilo " + std::to_string(this->thread_id) + " inicio:" + GREEN
    //                    + std::to_string(this->begin) + RESET + " - final: " + GREEN + std::to_string(this->end) +
    //                    RESET + "]";
    //for(unsigned i=0;i<this->coincidences.size();i++)
    while(!this->coincidences.empty())
    {
        format_line += commun_start_line + " :: línea " + MAGENTA + std::to_string(this->coincidences[i].line_number)
                       + RESET + " :: " + "... "
                       + this->coincidences.top().coincidence.previus_word + " "
                       + RED + this->coincidences.top().coincidence.word + RESET +
                       + " " + this->coincidences.top().coincidence.post_word + "\n";
        this->coincidences.pop();
    }
    return format_line;
}
void Client_Management::start_finding()
{

}