//
// Created by TOMIC on 26/04/2021.
//

#include "../include/Client_Management.h"

/*Client_Management::Client_Management(std::vector<Text> v_texts, std::string word,int fd_write_client,
                                        int initial_balance, std::string category)
{
    this->v_texts = v_texts;
    this->word = word;
    this->fd_write_client = fd_write_client;
    this->initial_balance = initial_balance;
    this->category;
    this->start_finding();
}*/

/*[[noreturn]] void Client_Management::wait_for_request()
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

        //Randomize number in order to choose 80% request for premium clients, and 20% normal client
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
                pattern = request_queue[i].word;
                fd_descriptor = request_queue[i].fd_descriptor;
                initial_balance = request_queue[i].initial_balance;
                v_txts = request_queue[i].v_texts;
                break;
            }
        }
        v_txts.erase(v_txts.begin() + i); //Remove request for queue
        extract_request_condition.notify_one();
        ul.unlock(); //Unlock the semaphore

        //Next, We divide the work by threads
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
        //Join the threads
        std::for_each(v_threads.begin(), v_threads.end(), [](std::thread& t) { t.join(); });

        //Get coincidences to Client
        if(get_number_coindicences() > 0){
            std::string coincidences_string_format = this->getCoincidences(fd_descriptor);
            write(fd_descriptor, coincidences_string_format.c_str(), sizeof(coincidences_string_format));
        } else
            write(fd_descriptor, "Sorry, We dont find coincidences.\n", 34);

        //Free resources in order to prepare next request
        this->free_resources();
    }
}*/


void free_resources() {
    //v_threads = std::vector<std::thread>();
    coincidences = std::priority_queue<Coincidence_Format, std::vector<Coincidence_Format>,
            myComp>();
    //v_texts = std::vector<Text>();
}

void add_coincidence(Coincidence_Format coincidence) {
    coincidences.push(coincidence);
}


int get_number_coindicences()
{
    return coincidences.size();
}
std::string getCoincidences(int get_coincidences){
    std::string format_line = "";
    std::string commun_start_line;
    while(!coincidences.empty())
    {
        format_line += commun_start_line + " :: línea " + MAGENTA + std::to_string(coincidences.top().line_number)
                       + RESET + " :: " + "... "
                       + coincidences.top().coincidence.previus_word + " "
                       + RED + coincidences.top().coincidence.word + RESET +
                       + " " + coincidences.top().coincidence.post_word + "\n";
        coincidences.pop();
    }
    return format_line;
}
void start_finding(std::vector<Text> v_texts, std::string word,int fd_write_client,
                   int initial_balance, std::string category, int client_pid)
{
    std::vector<std::thread> v_threads;
    int n_texts, max_threads_per_file,text_number, begin, end, text_lines, max_lines_per_thread, fd_descriptor;
    //Next, We divide the work by threads
    n_texts = v_texts.size();
    max_threads_per_file = (N_THREADS_PER_REPLIC / n_texts);

    if(max_threads_per_file == 0){
        fprintf(stderr,"[CLIENT_MANAGER %i]Error, the max lines per threads are 0. The client cant be attend\n",getpid());
        std::exit(EXIT_FAILURE);
    }
    text_number = 0;
    begin = 1;
    for(int j=0;j<N_THREADS_PER_REPLIC;j++){
        Text txt = v_texts[text_number];
        text_lines = txt.n_lines;
        max_lines_per_thread = (text_lines / max_threads_per_file);
        end =(begin + max_lines_per_thread) - 1;
        if((j % (max_threads_per_file-1)) == 0) end = text_lines;
        if(category == ILIMITED_PREMIUM_CATEGORY) v_threads.push_back(std::thread (find_ilimited_premium_client,j,txt,begin,end, word));

        if((j % (max_threads_per_file-1)) == 0){text_number++;begin=1;}
        else begin += max_lines_per_thread;
    }
    //Join the threads
    std::for_each(v_threads.begin(), v_threads.end(), [](std::thread& t) { t.join(); });

    //Get coincidences to Client
    if(get_number_coindicences() > 0){
        std::string coincidences_string_format = getCoincidences(fd_descriptor);
        write(fd_descriptor, coincidences_string_format.c_str(), sizeof(coincidences_string_format));
        if(kill(SIGUSR1,client_pid) == -1){
            fprintf(stderr,"[CLIENT_MANAGER %i]Error, We cannot send signal to Client %i\n",getpid(),client_pid);
            free_resources();
            std::exit(EXIT_FAILURE);
        }
    } else
        write(fd_descriptor, "Sorry, We dont find coincidences.\n", 34);
}
void find_ilimited_premium_client(int id, Text txt, int begin, int end, std::string pattern)
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

int main(int argc, char **argv)
{
    std::vector<Text> v_texts;
    std::string word;
    int fd_write_client;
    int initial_balance;
    std::string category;
    int client_pid;

    parse_argv(argc, argv,v_texts,&word, &fd_write_client, &initial_balance, &category, &client_pid);
    std::cout << "Client_Management creado" << std::endl;
    start_finding(v_texts,word, fd_write_client, initial_balance, category, client_pid);
}
void parse_argv(int argc, char **argv, std::vector<Text> &v_texts, std::string *word,
                int *fd_write_client,int *initial_balance, std::string *category, int *client_pid)
{
    if(argc < 7){
        fprintf(stderr,"[CLIENT_MANAGEMENT] Error, use: ./exec/Client_Management <category> <word> <initial_balance> <fd_pipe_descriptor> <client_pid> [text_files_name]");
        std::exit(EXIT_FAILURE);
    }
    *category = argv[1];
    *word = argv[2];
    *initial_balance = atoi(argv[3]);
    *fd_write_client = atoi(argv[4]);
    *client_pid = atoi(argv[5]);
    for(int i=6; i<argc; i++){
        if(fopen(argv[i],"r") != NULL){
            v_texts.push_back(Text(argv[i]));
        }
    }
}
