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
std::string getCoincidences(int client_pid){
    std::string format_line = "";
    while(!coincidences.empty())
    {
        format_line += "RESULTS FOR CLIENT: " + std::to_string(client_pid) + " " + BLUE + coincidences.top().get_file_name() + RESET + " :: línea " + MAGENTA + std::to_string(coincidences.top().line_number)
                       + RESET + " :: " + "... "
                       + coincidences.top().coincidence.previus_word + " "
                       + RED + coincidences.top().coincidence.word + RESET +
                       + " " + coincidences.top().coincidence.post_word + "\n";
        coincidences.pop();
    }
    return format_line;
    /*char *format_line = (char*)malloc(MAX_BUFFER_TEXT);
    //sprintf(format_line,"");
    strcpy(format_line, "");
    char aux[MAX_BUFFER_TEXT];
    while(!coincidences.empty())
    {
        sprintf(aux," :: line %d :: %s %s %s.\n",coincidences.top().line_number,(char *)coincidences.top().coincidence.previus_word.data(),
                (char *)coincidences.top().coincidence.word.data(), (char*)coincidences.top().coincidence.post_word.data());
             coincidences.pop();
             strcat(format_line,aux);
    }
    return format_line;*/
}
void start_finding(std::vector<Text> v_texts, char *word,
                   char *pipename, int initial_balance, char *category, int client_pid)
{
    int j;
    int clientpipe;
    std::vector<std::thread> v_threads;
    std::string coincidences_string_format;
    int n_texts, max_threads_per_file,text_number, begin, end, text_lines, max_lines_per_thread;
    //Open the client pipe
    clientpipe = open(pipename, O_WRONLY);

    //Next, We divide the work by threads

    n_texts = v_texts.size();
    max_threads_per_file = (N_THREADS_PER_REPLIC / n_texts);

    if(max_threads_per_file == 0){
        fprintf(stderr,"[CLIENT_MANAGER %i]Error, the max lines per threads are 0. The client cant be attend\n",getpid());
        std::exit(EXIT_FAILURE);
    }
    text_number = 0;
    begin = 1;

    for(j=1;j<=N_THREADS_PER_REPLIC;j++){
        Text txt = v_texts[text_number];
        text_lines = txt.n_lines;
        if(end != text_lines){
            max_lines_per_thread = (text_lines / max_threads_per_file);
            end =(begin + max_lines_per_thread) - 1;
            if((j % (max_threads_per_file)) == 0) end = text_lines;
            if(strncmp(category,ILIMITED_PREMIUM_CATEGORY,sizeof (category)) == 0)
                v_threads.push_back(std::thread (find_ilimited_premium_client,j,txt,begin,end, word));

            else if(strncmp(category,PREMIUM_CATEGORY,sizeof (category)) == 0)
                v_threads.push_back(std::thread (find_premium_client,j,txt,begin,end, word,initial_balance));

            else if(strncmp(category,NORMAL_CATEGORY,sizeof (category)) == 0)
                v_threads.push_back(std::thread (find_normal_client,j,txt,begin,end, word,initial_balance));

            if((j % (max_threads_per_file)) == 0){
                text_number++;
                if(text_number == v_texts.size()) text_number = v_texts.size()-1;
                begin=1;
            }
            else begin += max_lines_per_thread;
        }else {
            /*If the number of texts are odd, We fix the end*/
            text_number++;
            if(text_number >= v_texts.size()) break;
        }
    }
    //Join the threads
    std::for_each(v_threads.begin(), v_threads.end(), [](std::thread& t) { t.join(); });
    //Get coincidences to Client
    if(get_number_coindicences() > 0){
        //char *coincidences_string_format = getCoincidences();
        //std::cout << coincidences_string_format << std::endl;
        coincidences_string_format = getCoincidences(client_pid);
        /*Open the pipe*/
        write(clientpipe, coincidences_string_format.c_str(), coincidences_string_format.size());
        std::this_thread::sleep_for(std::chrono::milliseconds(600)); // Finalize the write
        std::cout << "[CLIENT MANAGEMENT " << getpid() << "] send the result to client: " << client_pid  << std::endl;
        if(kill(client_pid,SIGUSR1) == -1){
            fprintf(stderr,"[CLIENT_MANAGER %i]Error, We cannot send signal to Client %i\n",getpid(),client_pid);
            free_resources();
            std::exit(EXIT_FAILURE);
        }
    } else
        write(clientpipe, "Sorry, We dont find coincidences.\n", 34);

    close(clientpipe);
}
void find_normal_client(int id, Text txt, int begin, int end, std::string pattern, int initial_balance){
    std::vector<std::string> v_line_text;
    int position;
    int credits = initial_balance;

    /*Principal Metod of searching*/
    for(unsigned i=begin; i<=end && credits != 0;i++){
        v_line_text = txt.getlinevector(i);
        position = 0;
        while(position < v_line_text.size() && credits != 0){
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
                credits--;
            }
            position++;
        }
    }
}

void find_premium_client(int id, Text txt, int begin, int end, std::string pattern, int initial_balance){
    std::vector<std::string> v_line_text;
    int position;
    int credits = initial_balance;
    struct TPayment *payment;
    sem_t *sem_balance_ready, *sem_balance_charge, *sem_mutex;
    int shm_payment;
    /*Get shared memory segments and semaphores*/
    get_shm_segments(&shm_payment, &payment);
    get_sems(&sem_balance_ready, &sem_balance_charge, &sem_mutex);

    /*Principal Metod of searching*/
    for(unsigned i=begin; i<=end && credits != 0;i++){
        v_line_text = txt.getlinevector(i);
        position = 0;
        while(position < v_line_text.size() && credits != 0){
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
                credits--;
                if(credits == 0){
                    /*Recharge balance*/
                    wait_semaphore(sem_mutex);
                    payment->id = getppid();
                    payment->client_initial_balance = initial_balance;
                    payment->balance = credits;
                    std::cout << "[CLIENT MANAGEMENT " << getpid() << "] manda recargar puntos un proceso con 0 de balance y 10 de initial_balance" << std::endl;
                    signal_semaphore(sem_balance_ready);
                    wait_semaphore(sem_balance_charge);
                    std::cout << "[CLIENT MANAGEMENT " << getpid() << "] Saldo después de llamar a Payment_system = " << payment->balance << std::endl;
                    credits = payment->balance;
                    signal_semaphore(sem_mutex);
                }
            }
            position++;
        }
    }
}
void get_shm_segments(int *shm_payment, struct TPayment **p_payment)
{
    *shm_payment = shm_open(SHM_PAYMENT, O_RDWR, 0644);
    *p_payment = static_cast<TPayment *>(mmap(NULL, sizeof(struct TPayment),
                                              PROT_READ | PROT_WRITE, MAP_SHARED, *shm_payment, 0));
}

void get_sems(sem_t **p_sem_balance_ready, sem_t **p_sem_balance_charge, sem_t **p_mutex)
{
    *p_sem_balance_ready = get_semaphore(SEM_BALANCE_READY);
    *p_sem_balance_charge = get_semaphore(SEM_BALANCE_CHARGE);
    *p_mutex = get_semaphore(SEM_MUTEX);
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
    char *word;
    char *pipename;
    int initial_balance;
    char *category;
    int client_pid;

    parse_argv(argc, argv,v_texts,&word, &pipename, &initial_balance, &category, &client_pid);
    start_finding(v_texts, word, pipename, initial_balance, category, client_pid);
}
void parse_argv(int argc, char **argv, std::vector<Text> &v_texts, char **word,
                char **pipename,int *initial_balance, char **category, int *client_pid)
{
    int i;
    if(argc < 7){
        fprintf(stderr,"[CLIENT_MANAGEMENT] Error, use: ./exec/Client_Management <category> <word> <initial_balance> <fd_pipe_descriptor> <client_pid> [text_files_name]");
        std::exit(EXIT_FAILURE);
    }
    *category = argv[1];
    *word = argv[2];
    *initial_balance = atoi(argv[3]);
    *pipename = argv[4];
    *client_pid = atoi(argv[5]);
    for(i=6; i<argc; i++){
        v_texts.push_back(Text(argv[i]));
    }
}
