#include <iostream>
#include <string>
#include "input_parse.h"
#include "functionalities.h"
#include "data.h"

std::string zlang_input[MAXDIM];
int lines = 0;

void remove_whitespace(std::string& x) {
    size_t i = 0;
    while (i < x.size() && x[i] == ' ') {
        ++i;
    }
    x = x.substr(i);
}

void validate_input() {
    for (int i = 0; i < lines; ++i) {
        remove_whitespace(zlang_input[i]);
        proccess_line(zlang_input[i]);
    }
}

void output_input() {
    for (int i = 0; i < lines; ++i) {
        std::cout << zlang_input[i] << "\n";
    }
}

bool is_num( std::string& x) {
    try {
        size_t pos;
        std::stoi(x, &pos);
        return pos == x.size();
    } catch (...) {
        return false;
    }
}
