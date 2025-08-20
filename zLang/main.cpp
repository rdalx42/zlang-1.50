#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>

#include "input_parse.h"

std::ifstream input("main.zl");
std::ifstream zl_input("input.txt");

const char* pattern[] = {
    "@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@     @@  @@@@@@",
    "@@@@@@@@  @@@  @@@@@@",
    "@@@@@@@  @@@@  @@@@@@",
    "@@@ @@     @@     @@@",
    "@@@@@@@@@@@@@@@@@@@@@",
};

const int terminal_width = 80;
const int scale = 1;

void print_char_block(char c) {
    const char* color = (c == '@') ? "\033[33m" : "\033[37m"; 
    std::cout << color;
    for (int i = 0; i < scale; ++i)
        std::cout << c;
    std::cout << "\033[0m";
}

void print_scaled_line(const char* line) {
    int len = 0;
    while (line[len] != '\0') ++len;

    int scaled_width = len * scale;
    int left_padding = (terminal_width - scaled_width) / 2;
    if (left_padding < 0) left_padding = 0;

    for (int v = 0; v < scale; ++v) {
        for (int s = 0; s < left_padding; ++s) std::cout << ' ';
        for (int i = 0; i < len; ++i) {
            print_char_block(line[i]);
        }
        std::cout << '\n';
    }
}

void print_pattern() {
    for (int i = 0; i < 11; ++i) {
        print_scaled_line(pattern[i]);
    }
}

int main(void){
   
    print_pattern();

    if (!input) {
        std::cout << "Couldn't open main.zl file!\n";
        return 0;
    }

    std::string ln;

    std::cout << "\n======== [CODE INPUT] ========\n\n";

    output_program_input();

    std::cout << "\n======== [ZLANG OUTPUT] ========\n";

    auto start = std::chrono::high_resolution_clock::now();

    program_data.push_back(zLangProgram{}); 
    program_data[var_indx].name = "main.zl";
    RUN_ZLANG("main.zl", get_input_from_file("main.zl"), get_size_from_file("main.zl"));
    //std::cout << "RUNNING\n";
    clean_program_input("main.zl");

   // var_indx++;

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "\n\nProgram runtime: " << duration.count() << " milliseconds\n";

    std::cout << "\nTYPE 'END' TO EXIT\n";

    while (true) {
        std::getline(std::cin, ln);
        if (ln == "END") break;
        program_input[input_lines] = ln;
        ++input_lines;
    }

    return 0;
}
