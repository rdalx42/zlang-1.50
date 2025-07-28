
#include <string>
#include <cmath>
#include <climits>
#include <iostream>
#include <unordered_map>
#include "data.h"
#include "input_parse.h"

std::unordered_map<std::string, int> num_vars;
std::unordered_map<std::string, bool> bool_vars;
std::unordered_map<std::string, std::string> string_vars;
std::unordered_map<std::string, std::string> fn_contents;
std::unordered_map<std::string, std::vector<std::string>> fn_params;


int get_num_val(std::string& name){
    auto it = num_vars.find(name);
    if(it == num_vars.end()) return INT_MAX;
    return it->second;
}

std::string get_bool_val(std::string& name){
    if(bool_vars.find(name) == bool_vars.end()){
        std::cout << "VALUE DOESNT EXIST\n";
        return "ERR";
    }
    return bool_vars[name] ? "true" : "false";
}

std::string get_string_val(std::string& name){

    if(string_vars.find(name) == string_vars.end()){
        return "ERR";
    }

    return (string_vars[name]);
}

std::string get_var_type(std::string& name) {
    if (string_vars.find(name) != string_vars.end()) return "string type";
    if (num_vars.find(name) != num_vars.end()) return "num type";
    if (bool_vars.find(name) != bool_vars.end()) return "bool type";
    return "unknown type";
}


// will help A LOT

bool is_variable(std::string& name){
    if(num_vars.find(name) == num_vars.end() && string_vars.find(name) == string_vars.end() && bool_vars.find(name) == bool_vars.end()){
        return false;
    }
    return true;
}

void set_variable_to_data(std::string& varname, std::string& data_name){

    if(is_num(data_name)){
        num_vars[varname] = std::stoi(data_name);
        return;
    }

    if(num_vars.find(data_name)!=num_vars.end()){
        num_vars[varname] = num_vars[data_name];
        return;
    }

    if(data_name == "true" || data_name == "false"){
        // no built in cpp function for this.
        if(data_name == "true"){
            bool_vars[varname]=true;
        }else{bool_vars[varname]=false;}
        return;
    }

    if(bool_vars.find(data_name)!=bool_vars.end()){
        bool_vars[varname] = bool_vars[data_name];
        return;
    }

    std::cout<<data_name<<" isn't a variable, therefore it cannot be assigned to: "<<varname<<"\n";
}

void set_variable(std::string& name, std::string& value){
    if(num_vars.find(name) != num_vars.end()){
        if(is_num(value)){
            num_vars[name] = std::stoi(value);
        } else {
            std::cout << "COULDNT ASSIGN NUM TO NOT NUM TYPE\n";
            return;
        }
    } else if (bool_vars.find(name) != bool_vars.end()) {
        if(value == "true"||value=="1") {
            bool_vars[name] = true;
        } else if(value == "false"||value=="0") {
            bool_vars[name] = false;
        } else {
            std::cout << "COULDNT ASSIGN BOOL TO NOT BOOL TYPE\n";
            return;
        }
    } else if (string_vars.find(name) != string_vars.end()) {
        string_vars[name] = value;
    } else {
        std::cout << "VARIABLE DOES NOT EXIST\n";
    }
}
