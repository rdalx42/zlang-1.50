
#include <iostream>
#include <fstream>
#include <string>

#include "input_parse.h"

std::ifstream input("input.cbp");


int main(void) {
    if (!input) {
        std::cout << "Couldn't read - input doesn't exist.\n";
        return 0;
    }

    std::string ln;
    while (std::getline(input, ln)) {
        zlang_input[lines] = ln;
        ++lines;
    }

    validate_input();

    return 0;
}
