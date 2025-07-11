
#include <string>
#include <cmath>
#include <climits>
#include <iostream>
#include <unordered_map>
#include "data.h"
#include "input_parse.h"

std::unordered_map<std::string, int> num_vars;

int get_num_val(std::string& name){

    if(! num_vars[name]){
        //std::cout<<"ERR; VARIABLE " << name << "INVALID;";
        return INT_MAX;
    }

    return num_vars[name];
}

// will help A LOT

bool is_variable(std::string& name){
    if(!num_vars[name]){return false;}
    return true;
}

void set_variable(std::string& name, std::string& value){
    std::cout<<"value > "<<value<<"\n";
    if(num_vars[name]){
        if(is_num(value)){
            num_vars[name] = std::stoi(value);
        }else{
            std::cout<<"COULDNT ASSIGN NUM TO NOT NUM TYPE\n";
            return;
        }
    }
}
