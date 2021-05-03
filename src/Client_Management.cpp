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
    std::vector<Text> thread_texts;
    int n_texts, max_threads_per_file, text_number, text_lines, max_lines_per_thread;
    int random_number;
    int begin, end;
    bool vip;
    bool choose = false;

    srand(time(NULL));

attend_repics:
    for(;;)
    {
        std::unique_lock<std::mutex> ul(queue_semaphore_management);

        extract_request_condition.wait(ul,[]{
            return request_queue.size() > 0;
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
                fd_descriptor = request_queue[i].fd_descriptor;
                initial_balance = request_queue[i].initial_balance;
                v_txts = request_queue[i].v_texts;
                break;
            }
        }
        v_txts.erase(v_txts.begin() + i); /*Remove request for queue*/
        extract_request_condition.notify_one();
        ul.unlock(); /*Unlock the semaphore*/

        /*Next, We divide the work by threads*/
        n_texts = v_txts.size();
        max_threads_per_file = (N_REPLICS / n_texts);

        if(max_threads_per_file == 0){
            fprintf(stderr,"[CLIENT_MANAGER %i]Error, the max lines per threads are 0. The client cant be attend\n",getthreadid());
            goto attend_repics;
        }
        text_number = 0;
        begin = 1;
        for(int j=0;j<N_REPLICS;j++){
            Text txt = v_txts[text_number];
            text_lines = txt.n_lines;
            max_lines_per_thread = (text_lines / max_threads_per_file);
            end =(begin + max_lines_per_thread) - 1;
            if((j % (max_threads_per_file-1)) == 0) end = text_lines;
            v_threads.push_back(std::thread (&Client_Management::start_finding,this,j,category,txt,begin,end, pattern));
            if((j % (max_threads_per_file-1)) == 0){text_number++;begin=1;}
        }
        /*Join the threads*/
        std::for_each(v_threads.begin(), v_threads.end(), [](std::thread& t) { t.join(); });

        /*Get coincidences to Client*/
        if(get_number_coindicences() > 0){
            std::string coincidences_string_format = this->getCoincidences(fd_descriptor);
            write(fd_descriptor, coincidences_string_format.c_str(), sizeof(coincidences_string_format));
        } else
            write(fd_descriptor, "Sorry, We dont find coincidences.\n", 34);

        /*Free resources in order to prepare next request*/
        this->free_resources();
    }
}

Client_Management::~Client_Management()
{
    /*Free vectors*/
    this->free_resources();
}

void Client_Management::free_resources() {
    this->v_threads = std::vector<std::thread>();
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
std::string Client_Management::getCoincidences(int get_coincidences){
    std::string format_line = "";
    std::string commun_start_line;
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
void Client_Management::start_finding(int id, std::string category, Text txt, int begin, int end, std::string pattern)
{
    if(category == ILIMITED_PREMIUM_CATEGORY) find_ilimited_premium_client(id, txt, begin, end, pattern);

}
void Client_Management::find_ilimited_premium_client(int id, Text txt, int begin, int end, std::string pattern)
{
    std::vector<std::string> v_line_text;
    int position;
    /*Principal Metod of searching*/
    for(unsigned i=begin; i<=end;i++){
        v_line_text = txt.getlinevector(i);
        position = 0;
        while(position < v_line_text.size()){
            if(txt.wordwrapper(v_line_text[position]).compare(txt.wordwrapper(pattern)) == 0){
                Coincidence_Format coin(i,pattern,txt.file_name);
                /*find the pattern, next search the previous and post word*/
                if(position == (v_line_text.size()-1)){
                    /*The pattern is at finnish*/
                    coin.set_coincidence(v_line_text[position-1],v_line_text[position],"");
                }else if(position == 0){
                    /*The pattern is at beginning*/
                    coin.set_coincidence("",v_line_text[position],v_line_text[position+1]);
                }else{
                    coin.set_coincidence(v_line_text[position-1],v_line_text[position],v_line_text[position+1]);
                }
                mutex.lock();
                add_coincidence(coin);
                mutex.unlock();
            }
            position++;
        }
    }
}