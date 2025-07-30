
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

std::vector<std::string> space_tokens = {
    "=",
    "+",
    "-",
    "/",
    "*",
    "return",
    "color",
    "is",
    "isnt",
    "true",
    "false",
    ":",
    "{",
    "}",
};

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

void clean_program_input() {

    // clean up lines so the interpreter can use sstream to read input.
    // TODO: when adding strings make sure you don't sepparate expressions in strings.

    for (int l = 0; l < lines; ++l) {
        std::string current_ln = zlang_input[l];

        for (const std::string& token : space_tokens) {
            size_t pos = 0;
            while ((pos = current_ln.find(token, pos)) != std::string::npos) {
                if (pos == 0 || current_ln[pos - 1] != ' ')
                    current_ln.insert(pos, " "), ++pos;
                pos += token.length();
                if (pos >= current_ln.size() || current_ln[pos] != ' ')
                    current_ln.insert(pos, " ");
            }
        }

        zlang_input[l] = current_ln;
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

bool is_fn_value(std::string& x){
    size_t colonpos = x.find(":");
    if(colonpos!=std::string::npos){
        std::string fn_name = x.substr(1,x.size());
        if(get_function_read_info_arr_indx(fn_name)!=-1){return true;}
    }return false;
}
