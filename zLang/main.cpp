#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>

#include "input_parse.h"

std::ifstream input("main.zl");
std::ifstream zlinput("input.txt");

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

const int TERMINAL_WIDTH = 80;
const int SCALE = 1;
const int DELAY_MS = 1;

void printCharBlock(char c) {
    const char* color = (c == '@') ? "\033[33m" : "\033[37m"; 
    std::cout << color;
    for (int i = 0; i < SCALE; ++i)
        std::cout << c;
    std::cout << "\033[0m";
}

void printScaledLine(const char* line) {
    int len = 0;
    while (line[len] != '\0') ++len;

    int scaledWidth = len * SCALE;
    int leftPadding = (TERMINAL_WIDTH - scaledWidth) / 2;
    if (leftPadding < 0) leftPadding = 0;

    for (int v = 0; v < SCALE; ++v) {
        for (int s = 0; s < leftPadding; ++s) std::cout << ' ';
        for (int i = 0; i < len; ++i) {
            printCharBlock(line[i]);
        }
        std::cout << '\n';
    }
}

void animatePatternScaled() {
    using namespace std::chrono_literals;

    for (int i = 0; i < 3; ++i) std::cout << '\n';

    for (int i = 0; i < 11; ++i) {
        const char* line = pattern[i];
        int len = 0;
        while (line[len] != '\0') ++len;

        int scaledWidth = len * SCALE;
        int leftPadding = (TERMINAL_WIDTH - scaledWidth) / 2;
        if (leftPadding < 0) leftPadding = 0;

        for (int v = 0; v < SCALE; ++v) {
            for (int s = 0; s < leftPadding; ++s) std::cout << ' ';

            for (int i = 0; i < len; ++i) {
                char c = line[i];

                for (int h = 0; h < SCALE; ++h) {
                    printCharBlock(c);
                    std::cout.flush();
                    std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
                }
            }
            std::cout << '\n';
        }
    }
    std::cout << "\n\n";
}

int main(void) {
    
    animatePatternScaled();

    if(!input){
        std::cout<<"couldn't open main.zl file!\n";
        return 0;
    }

    std::string ln;

    std::cout << "\n======== [CODE INPUT] ========\n\n";

    output_program_input();

    std::cout << "\n======== [ZLANG OUTPUT] ========\n";

    auto start = std::chrono::high_resolution_clock::now();

    program_data.push_back(zLangProgram{}); 
    program_data[var_indx].name = "main.zl";
   // program_data[var_indx].has_ran = true;
    RUN_ZLANG("main.zl", get_input_from_file("main.zl"), get_size_from_file("main.zl"));

    var_indx++;

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "\n\nProgram runtime: " << duration.count() << " milliseconds\n";

    std::cout<<"\nTYPE 'END' TO EXIT\n";
    
    while (true) {
        std::getline(std::cin, ln);
        if (ln == "END") break;
        program_input[input_lines] = ln;
        ++input_lines;
    }
    return 0;
}
