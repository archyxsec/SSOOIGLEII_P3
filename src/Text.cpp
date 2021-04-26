//
// Created by TOMIC on 26/04/2021.
//

#include "../include/Text.h"


Text::Text(std::string filename){
    this->file_name = filename;
    this->n_lines = get_number_of_lines();
}

unsigned Text::get_number_of_lines(){
    unsigned number_of_lines;
    std::string line;
    std::ifstream myfile(this->file_name);
    while (std::getline(myfile, line, '\n'))
        ++number_of_lines;

    return number_of_lines;
}

std::string Text::getlinetext(int n_line)
{
    std::string line;
    std::ifstream myfile(this->file_name);
    int i = 0;
    while (std::getline(myfile, line, '\n')){
        if(++i == n_line) break;
    }
    /*Out of bounds of line check*/
    if(i==n_line) return line;
    else return "";
}

std::vector<std::string> Text::getlinevector(int n_line)
{
    std::string linetext = getlinetext(n_line);
    char* cstr=const_cast<char*>(linetext.c_str());
    char* current;
    std::string sep = " ";
    std::vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}
std::string Text::wordwrapper(std::string word)
{
    std::string formatedword = "";
    char character;
    for(unsigned i=0; i<word.size(); i++)
    {
        character = word[i];

        if(i==0 || i==word.size()-1){
            if(extraer(character))
                continue;
        }

        /*Check tildes changing ASCII code*/
        if(character == -61 && (word[i+1] == -95 || word[i+1] == -127)){
            character = 97;
            i++;
        }else if(character == -61 && (word[i+1] == -87 || word[i+1] == -119)){
            character = 101;
        }else if(character == -61 && (word[i+1] == -83 || word[i+1] == -115)){
            character = 105;
            i++;
        }
        else if(character == -61 && (word[i+1] == -77 || word[i+1] == -109)){
            character = 111;
            i++;
        }else if(character == -61 && (word[i+1] == -70 || word[i+1] == -102)){
            character = 117;
            i++;
        }

        formatedword += tolower(character);

    }
    return formatedword;
}

bool Text::extraer(char character)
{
    bool result = false;
    /*Check if character is in range of symbols in ASCII Table*/
    if(character >= 33 && character <= 47) result = true;
    else if(character >= 58 && character <= 64) result = true;
    else if(character >= 91 && character <= 96) result = true;
    else if(character >= 123 && character <= 126) result = true;
    return result;
}