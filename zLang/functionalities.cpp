#include <iostream>
#include <string>
#include <cmath>
#include <climits>
#include <sstream>
#include "input_parse.h"
#include "data.h"

int operation_statement(std::string x, std::string op, std::string y) {
    int val_x = 0;
    int val_y = 0;

    if (is_num(x)) {
        val_x = std::stoi(x);
    } else if (get_num_val(x) != INT_MAX) {
        val_x = num_vars[x];
    } else {
        return 0;
    }

    if (y == "") {
        return val_x;
    }

    if (is_num(y)) {
        val_y = std::stoi(y);
    } else if (get_num_val(y) != INT_MAX) {
        val_y = num_vars[y];
    } else {
        return 0;
    }

    if (op == "+=") {
        if (get_num_val(x) != INT_MAX) {
            num_vars[x] += val_y;
            return num_vars[x];
        }
    } else if (op == "-=") {
        if (get_num_val(x) != INT_MAX) {
            num_vars[x] -= val_y;
            return num_vars[x];
        }
    } else if (op == "*=") {
        if (get_num_val(x) != INT_MAX) {
            num_vars[x] *= val_y;
            return num_vars[x];
        }
    } else if (op == "/=") {
        if (val_y == 0) {
            std::cout << "Division by zero error\n";
            return 0;
        }
        if (get_num_val(x) != INT_MAX) {
            num_vars[x] /= val_y;
            return num_vars[x];
        }
    } else if (op == "%=") {
        if (val_y == 0) {
            std::cout << "Modulo by zero error\n";
            return 0;
        }
        if (get_num_val(x) != INT_MAX) {
            num_vars[x] %= val_y;
            return num_vars[x];
        }
    } else if (op == "&=") {
        if (get_num_val(x) != INT_MAX) {
            num_vars[x] &= val_y;
            return num_vars[x];
        }
    } else if (op == "|=") {
        if (get_num_val(x) != INT_MAX) {
            num_vars[x] |= val_y;
            return num_vars[x];
        }
    } else if (op == "^=") {
        if (get_num_val(x) != INT_MAX) {
            num_vars[x] ^= val_y;
            return num_vars[x];
        }
    } else if (op == "<<=") {
        if (get_num_val(x) != INT_MAX) {
            num_vars[x] <<= val_y;
            return num_vars[x];
        }
    } else if (op == ">>=") {
        if (get_num_val(x) != INT_MAX) {
            num_vars[x] >>= val_y;
            return num_vars[x];
        }
    } else if (op == "+") {
        return val_x + val_y;
    } else if (op == "-") {
        return val_x - val_y;
    } else if (op == "*") {
        return val_x * val_y;
    } else if (op == "/") {
        if (val_y == 0) {
            std::cout << "Division by zero error\n";
            return 0;
        }
        return val_x / val_y;
    } else if (op == "%") {
        if (val_y == 0) {
            std::cout << "Modulo by zero error\n";
            return 0;
        }
        return val_x % val_y;
    } else if (op == "&") {
        return val_x & val_y;
    } else if (op == "|") {
        return val_x | val_y;
    } else if (op == "^") {
        return val_x ^ val_y;
    } else if (op == "<<") {
        return val_x << val_y;
    } else if (op == ">>") {
        return val_x >> val_y;
    }

    return LLONG_MAX;
}

void proccess_line(std::string& x){

    std::stringstream ss(x);
    std::string start_keyword,name,eq,op,v,y,z;

    ss >> start_keyword;

    if(start_keyword == "nr"){
        ss >> name >> eq >> v;
        if (!(ss >> y >> z)) {
            y = "";
            z = "";
        }

        if(name != "" && eq == "="){
            int val;
            if(y == ""){
                val = operation_statement(v, "", "");
            } else {
                val = operation_statement(v, y, z);
            }

            if( val < INT_MAX && val >= INT_MIN){
                num_vars[name] = val;
            } else {
                std::cout << "NR VALUE EXCEEDS LIMIT\n";
                return;
            }
        } else {
            std::cout << "NR COULDN'T BE DECLARED\n";
            return;
        }
    } else if(start_keyword == "print") {
        std::string line;
        std::getline(ss, line);

        size_t i = 0;
        while (i < line.size() && isspace(line[i])) {
            ++i;
        }

        for (; i < line.size(); ++i) {

            if((i+1)<=line.size()){
                if(line[i]=='!'&&std::tolower(line[i+1])=='n'){
                    std::cout<<"\n";
                    i+=2; // skip line
                }
            }

            if (line[i] == '$') {
                std::string varname = "";
                ++i;
                while (i < line.size() && !isspace(line[i])) {
                    varname += line[i];
                    ++i;
                }
                --i;
                if (get_num_val(varname)) {
                    std::cout << num_vars[varname];
                } else {
                    std::cout << varname;
                }
            } else {
                std::cout << line[i];
            }
        }
        std::cout << std::endl;
    }else {
        ss >> op >> z;
        operation_statement(start_keyword, op, z);
    }
}
