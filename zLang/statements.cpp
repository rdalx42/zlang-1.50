
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
        if(is_fn_value(x)==true){
            x=x.substr(1,x.size());
            if(is_num(fn_values[x]) == true){
                val_x = std::stoi(fn_values[x]);
            }else if(num_vars.find(x)!=num_vars.end()){
                val_x = num_vars[x];
            }else{
                return 0;
            }
        }
    }

    if (y.empty()) {
        return val_x;
    }

    if (is_num(y)) {
        val_y = std::stoi(y);
    } else if (get_num_val(y) != INT_MAX) {
        val_y = num_vars[y];
    } else {
        if(is_fn_value(y)==true){
            y=y.substr(1,y.size());
            if(is_num(fn_values[y]) == true){
                val_y = std::stoi(fn_values[y]);
            }else if(num_vars.find(y)!=num_vars.end()){
                val_y = num_vars[y];
            }else{
                return 0;
            }
        }
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

bool bool_statement(std::string x, std::string op, std::string y) {

    int yval = INT_MAX;
    int xval = INT_MAX;

    if (is_num(y)) {
        yval = std::stoi(y);
    } else if (num_vars.find(y) != num_vars.end()) {
        yval = num_vars[y];
    } else if (is_fn_value(y)) {
        std::string cy = y.substr(1, y.size());
        if (fn_values.find(cy) != fn_values.end()) {
            std::string val = fn_values[cy];
            if (is_num(val)) {
                yval = std::stoi(val);
            } else if (num_vars.find(val) != num_vars.end()) {
                yval = num_vars[val];
            }
        }
    }

    if (is_num(x)) {
        xval = std::stoi(x);
    } else if (num_vars.find(x) != num_vars.end()) {
        xval = num_vars[x];
    } else if (is_fn_value(x)) {
        std::string cx = x.substr(1, x.size());
        if (fn_values.find(cx) != fn_values.end()) {
            std::string val = fn_values[cx];
            if (is_num(val)) {
                xval = std::stoi(val);
            } else if (num_vars.find(val) != num_vars.end()) {
                xval = num_vars[val];
            }
        }
    }

    if (xval != INT_MAX && yval != INT_MAX) {
        if (op == "is") return xval == yval;
        if (op == "isnt" || op == "isn't") return xval != yval;
        if (op == ">=") return xval >= yval;
        if (op == "<=") return xval <= yval;
        if (op == ">") return xval > yval;
        if (op == "<") return xval < yval;
    }

    bool xb = false, yb = false;
    bool x_set = false, y_set = false;

    if (x == "true") {
        xb = true;
        x_set = true;
    } else if (x == "false") {
        xb = false;
        x_set = true;
    } else if (bool_vars.find(x) != bool_vars.end()) {
        xb = bool_vars[x];
        x_set = true;
    } else if (is_fn_value(x)) {

        std::string cx = x.substr(1, x.size());
        if (fn_values.find(cx) != fn_values.end()) {

            std::string val = fn_values[cx];
            if (val == "true") {
                xb = true;
                x_set = true;
            } else if (val == "false") {
                xb = false;
                x_set = true;
            } else if (bool_vars.find(val) != bool_vars.end()) {
                xb = bool_vars[val];
                x_set = true;
            }
        }
    }

    if (y == "true") {
        yb = true;
        y_set = true;
    } else if (y == "false") {
        yb = false;
        y_set = true;
    } else if (bool_vars.find(y) != bool_vars.end()) {
        yb = bool_vars[y];
        y_set = true;
    } else if (is_fn_value(y)) {
        std::string cy = y.substr(1, y.size());
        if (fn_values.find(cy) != fn_values.end()) {
            std::string val = fn_values[cy];
            if (val == "true") {
                yb = true;
                y_set = true;
            } else if (val == "false") {
                yb = false;
                y_set = true;
            } else if (bool_vars.find(val) != bool_vars.end()) {
                yb = bool_vars[val];
                y_set = true;
            }
        }
    }

    if (x_set && y_set) {

        if (op == "is") return xb == yb;
        if (op == "isnt" || op == "isn't") return xb != yb;
    }

    if(x_set && !y_set){
        return xb;
    }

    return false;
}
