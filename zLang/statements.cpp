
#include <iostream>
#include <algorithm>
#include <string>
#include <climits>
#include "statemenets.h"
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

    if (y.empty()) {
        return val_x;
    }

    if (is_num(y)) {
        val_y = std::stoi(y);
    } else if (get_num_val(y) != INT_MAX) {
        val_y = num_vars[y];
    } else {
        return 0;
    }

    if (op == "+=" && get_num_val(x) != INT_MAX) {
        num_vars[x] += val_y;
        return num_vars[x];
    }
    if (op == "-=" && get_num_val(x) != INT_MAX) {
        num_vars[x] -= val_y;
        return num_vars[x];
    }
    if (op == "*=" && get_num_val(x) != INT_MAX) {
        num_vars[x] *= val_y;
        return num_vars[x];
    }
    if (op == "/=") {
        if (val_y == 0) {
            std::cout << "Division by zero error\n";
            return 0;
        }
        if (get_num_val(x) != INT_MAX) {
            num_vars[x] /= val_y;
            return num_vars[x];
        }
    }
    if (op == "%=") {
        if (val_y == 0) {
            std::cout << "Modulo by zero error\n";
            return 0;
        }
        if (get_num_val(x) != INT_MAX) {
            num_vars[x] %= val_y;
            return num_vars[x];
        }
    }
    if (op == "&=" && get_num_val(x) != INT_MAX) {
        num_vars[x] &= val_y;
        return num_vars[x];
    }
    if (op == "|=" && get_num_val(x) != INT_MAX) {
        num_vars[x] |= val_y;
        return num_vars[x];
    }
    if (op == "^=" && get_num_val(x) != INT_MAX) {
        num_vars[x] ^= val_y;
        return num_vars[x];
    }
    if (op == "<<=" && get_num_val(x) != INT_MAX) {
        num_vars[x] <<= val_y;
        return num_vars[x];
    }
    if (op == ">>=" && get_num_val(x) != INT_MAX) {
        num_vars[x] >>= val_y;
        return num_vars[x];
    }
    if (op == "+") return val_x + val_y;
    if (op == "-") return val_x - val_y;
    if (op == "*") return val_x * val_y;
    if (op == "/") {
        if (val_y == 0) {
            std::cout << "Division by zero error\n";
            return 0;
        }
        return val_x / val_y;
    }
    if (op == "%") {
        if (val_y == 0) {
            std::cout << "Modulo by zero error\n";
            return 0;
        }
        return val_x % val_y;
    }
    if (op == "&") return val_x & val_y;
    if (op == "|") return val_x | val_y;
    if (op == "^") return val_x ^ val_y;
    if (op == "<<") return val_x << val_y;
    if (op == ">>") return val_x >> val_y;

    return LLONG_MAX;
}

bool bool_statement(std::string x, std::string op, std::string y){

    // x and y must be same type until we add functions - which will probably not happen really soon

    int yval=INT_MAX;
    int xval=INT_MAX;

    if(is_num(y)){
        yval = std::stoi(y);
    }else if(num_vars.find(y)!=num_vars.end()){
        yval = num_vars[y];
    }

    if(is_num(x)){
        xval = std::stoi(x);
    }else if(num_vars.find(x)!=num_vars.end()){
        xval = num_vars[x];
    }

    if(yval!=INT_MAX&&xval!=INT_MAX){ // knowing that the biggest nr limit = INT_MAX - 1

        if(op == "is"){
            return xval == yval;
        }

        if(op == "isnt"){
            return xval != yval;
        }

        if(op == ">="){
            return xval >= yval;
        }

        if(op == "<="){
            return xval <= yval;
        }

        if(op == ">"){
            return xval > yval;
        }

        if(op == "<"){
            return xval < yval;
        }
    }

    return false;
}
