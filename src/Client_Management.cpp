/******************************************************
* Project : Práctica 3 de Sistemas Operativos II
*
* Program name : Client_Management.cpp
*
* Author : Tomás Jesús García López y Sergio Sánchez Iglesias
*
* Date created : 26/04/2021
*
* Purpose : Manejador de clientes, su respectiva busquedas
*
*
* Revision History:
*
* Date          Author          Ref      Revision
* 29/04/2021    Tomás           1        Empezando a programar funciones
* 01/05/2021    Tomás           2        Función wait_for_request cambiado
* 03/05/2021    Tomás           3        Tocada Función wait_for_request y empezado start_finding()
* 03/05/2021    Tomás           4        función find_ilimited_premium_client y MERGE en start_finding de otra rama
* 05/05/2021    Tomás           5        Cambiada clase por funciones, modificado start_finding() y creado main()
* 07/05/2021    Sergio          6        Intentando avisar al cliente de que estan los resultados
* 07/05/2021    Tomás           7        Función start_finding() modificada y parse_argv
* 10/05/2021    Tomás           8        Cambiado getcoincidences y start_finding. Creado hilos para busquedas.
*                                        Implementado find_normal_client, find_premium_client, get_shm_segments y get_sems
*
|********************************************************/

#include "../include/Client_Management.h"

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
    return EXIT_SUCCESS;
}

/********************************** ARGUMENT MANAGEMENT *************************************************/

void parse_argv(int argc, char **argv, std::vector<Text> &v_texts, char **word,
                char **pipename,int *initial_balance, char **category, int *client_pid)
{
    int i;
    if(argc < 7){
        fprintf(stderr,"%s[CLIENT_MANAGEMENT] Error%s, use: ./exec/Client_Management <category> <word> "
                       "<initial_balance> <fd_pipe_descriptor> <client_pid> [text_files_name]",RED,RESET);
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

/*********************************** SEARCH MANAGEMENT **************************************************/

void start_finding(std::vector<Text> v_texts, char *word,
                   char *pipename, int initial_balance, char *category, int client_pid)
{
    int j;
    int clientpipe;
    sem_t *sem_replic_finish = get_semaphore(SEM_REPLIC_FINISH);
    std::vector<std::thread> v_threads;
    std::string coincidences_string_format;
    int n_texts, max_threads_per_file,text_number, begin, end, text_lines, max_lines_per_thread;
    std::promise<int> prom;
    std::future<int> fut;

    //Open the client pipe
    clientpipe = open(pipename, O_WRONLY);

    //Next, We divide the work by threads
    n_texts = v_texts.size();
    max_threads_per_file = (N_THREADS_PER_REPLIC / n_texts);

    if(max_threads_per_file == 0){
        fprintf(stderr,"%s[CLIENT_MANAGER %i]Error%s, the max lines per threads are 0. The client cant be attend\n",RED,getpid(),RESET);
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
                v_threads.push_back(std::thread (find_ilimited_premium_client,j,txt,begin,end, word, std::ref(prom)));

            else if(strncmp(category,PREMIUM_CATEGORY,sizeof (category)) == 0)
                v_threads.push_back(std::thread (find_premium_client,j,txt,begin,end, word,initial_balance,std::ref(prom)));

            else if(strncmp(category,NORMAL_CATEGORY,sizeof (category)) == 0)
                v_threads.push_back(std::thread (find_normal_client,j,txt,begin,end, word,initial_balance,std::ref(prom)));

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
    try{
        fut = prom.get_future();
        if(get_number_coindicences() > 0){
            coincidences_string_format = getCoincidences(client_pid);
            /*Open the pipe*/
            write(clientpipe, coincidences_string_format.c_str(), coincidences_string_format.size());
            std::this_thread::sleep_for(std::chrono::milliseconds(400)); // Finalize the write
            std::cout << BOLDYELLOW << "[CLIENT MANAGEMENT " << RESET << YELLOW << getpid() << RESET << BOLDYELLOW
                <<"]" << RESET << BOLDWHITE
                << " send the result to client: " << RESET << BOLDCYAN << client_pid  << RESET << std::endl;
        } else
            write(clientpipe, "Sorry, We dont find coincidences.\n", 34);

        close(clientpipe);
    }catch(std::exception& e){
        std::cout << BOLDRED << "[CLIENT MANAGEMENT " << getpid() << "] caught an exception: " << RESET << e.what() << std::endl;
    }

    /* Signal to Buscador, in order to tell finnish */
    signal_semaphore(sem_replic_finish);
}
void find_normal_client(int id, Text txt, int begin, int end, std::string pattern,
                        int initial_balance, std::promise<int>& prom){
    std::vector<std::string> v_line_text;
    int position;
    int credits = initial_balance;

    try{
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
    }catch(std::exception& ){
        prom.set_exception(std::current_exception());
    }
}

void find_premium_client(int id, Text txt, int begin, int end, std::string pattern,
                         int initial_balance, std::promise<int>& prom){
    std::vector<std::string> v_line_text;
    int position;
    int credits = initial_balance;
    struct TPayment *payment;
    sem_t *sem_balance_ready, *sem_balance_charge, *sem_mutex;
    int shm_payment;
    /*Get shared memory segments and semaphores*/
    get_shm_segments(&shm_payment, &payment);
    get_sems(&sem_balance_ready, &sem_balance_charge, &sem_mutex);

    try{
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
                        std::cout << BOLDWHITE << "[CLIENT MANAGEMENT " << getpid() << "] " << RESET << "manda recargar puntos un proceso con "
                            << RED << "0" << RESET " de balance y " << BOLDYELLOW << initial_balance << RESET
                            <<" creditos de initial_balance" << std::endl;
                        signal_semaphore(sem_balance_ready);
                        wait_semaphore(sem_balance_charge);
                        std::cout << "[CLIENT MANAGEMENT " << getpid() << "] Saldo después de llamar a Payment_system = "
                        << GREEN << payment->balance << RESET << std::endl;
                        credits = payment->balance;
                        signal_semaphore(sem_mutex);
                    }
                }
                position++;
            }
        }
    }catch(std::exception& ){
        prom.set_exception(std::current_exception());
    }
}

void find_ilimited_premium_client(int id, Text txt, int begin, int end, std::string pattern, std::promise<int>& prom)
{
    std::vector<std::string> v_line_text;
    int position;
    /*Principal Metod of searching*/
    try{
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
    }catch(std::exception& ){
        prom.set_exception(std::current_exception());
    }
}

/****************************** COINCIDENCE MANAGEMENT ****************************************/

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
        format_line += MAGENTA + std::string("RESULTS FOR CLIENT: ") + RESET + BOLDMAGENTA + std::to_string(client_pid)
                + " " + RESET + BLUE + coincidences.top().get_file_name() + RESET + " :: línea " + MAGENTA + std::to_string(coincidences.top().line_number)
                       + RESET + " :: " + "... "
                       + coincidences.top().coincidence.previus_word + " "
                       + RED + coincidences.top().coincidence.word + RESET +
                       + " " + coincidences.top().coincidence.post_word + "\n";
        coincidences.pop();
    }
    return format_line;
}

/********************************* SEMAPHORES AND SHARED MEMORY MANAGEMENT ********************************************/

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

/******** FREE RESOURCES *********/

void free_resources() {
    coincidences = std::priority_queue<Coincidence_Format, std::vector<Coincidence_Format>,
            myComp>();
}
