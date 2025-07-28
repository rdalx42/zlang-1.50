#include <iostream>
#include <string>
#include "input_parse.h"
#include "functionalities.h"
#include "data.h"
std::string zlang_input[MAXDIM];
std::string program_input[MAXDIM];

int lines = 0;
int input_lines = 0;
int current_zlang_input_line = 0;

void remove_whitespace(std::string& x) {
    size_t i = 0;
    while (i < x.size() && x[i] == ' ') {
        ++i;
    }
    x = x.substr(i);
}

void RUN_ZLANG() {
    for (int i = 0; i < lines; ++i) {
        remove_whitespace(zlang_input[i]);
        proccess_line(zlang_input[i],i);
    }
}

void output_input() {
    for (int i = 0; i < lines; ++i) {
        std::cout << zlang_input[i] << "\n";
    }
}

void output_program_input(){
    for(int i = 0 ; i < input_lines; ++ i){
        std::cout<<"LINE: "<<i+1<<": "<<program_input[i]<<"\n";
        std::cout<<"--------";
        for(int j = 0; j < program_input[i].size();++j){
            std::cout<<"-";
        }std::cout<<"\n";
    }
}

bool is_num(std::string& x) {
    try {
        size_t pos;
        std::stoi(x, &pos);
        return pos == x.size();
    } catch (...) {
        return false;
    }
}

bool is_bool(std::string& x){
    return(x == "true" || x == "false");
}

