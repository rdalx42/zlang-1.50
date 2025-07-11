
#include <string>
#include <cmath>
#include <climits>
#include <iostream>
#include <unordered_map>
#include "data.h"

std::unordered_map<std::string, int> num_vars;

int get_num_val(std::string& name){

    if(! num_vars[name]){
        std::cout<<"ERR; VARIABLE " << name << "INVALID;";
        return INT_MAX;
    }

    return num_vars[name];
}
