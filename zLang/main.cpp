#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "input_parse.h"

std::ifstream input("input.cbp");

int main(void) {

    if (!input) {
        return 0;
    }

    std::cout << "\n======== [WRITE PROGRAM INPUT - PRESS CTRL + C AND ENTER TO FINISH] ========\n";

    // get code input from file

    std::string ln;

    while (std::getline(input, ln)) {
        zlang_input[lines] = ln;
        ++lines;
    }

    // get user input from console and use it as zlang program input

    std::string word;

    while (std::cin >> word) {
        program_input[input_lines] = word;
        ++input_lines;
    }

    std::cout<<"\n======== [CODE INPUT] ========\n\n";

    output_program_input();

    std::cout<<"\n======== [ZLANG OUTPUT] ========\n";

    RUN_ZLANG(); // run code after input was written

    return 0;
}
