#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>

#include "input_parse.h"

std::ifstream input("main.zl");

const char* pattern[] = {
    "@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@              @",
    "@@@@@@@@@@@@@@@@@@  @",
    "@@@@@@@@@@@@@@@@   @@",
    "@@@@@@@@@@@@@@    @@@",
    "@@@@@@@@@@@@     @@@@",
    "@@@@@@@@@@      @@@@@",
    "@@@@@@@@       @@@@@@",
    "@@@@@@        @@@@@@@",
    "@@@@@        1.30 @@@",
    "@@@@@@@@@@@@@@@@@@@@@",
};

const int TERMINAL_WIDTH = 80;
const int SCALE = 1;
const int DELAY_MS = 1;

void printCharBlock(char c) {
    char printChar = (c == '@') ? '@' : ' ';
    for (int i = 0; i < SCALE; ++i)
        std::cout << printChar;
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
                char printChar = (c == '@') ? '@' : ' ';
                for (int h = 0; h < SCALE; ++h) {
                    std::cout << printChar;
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

    if (!input) {
        return 0;
    }

    std::cout << "\n======== [WRITE PROGRAM INPUT - PRESS CTRL + C AND ENTER TO FINISH] ========\n";

    std::string ln;
    while (std::getline(input, ln)) {
        zlang_input[lines] = ln;
        ++lines;
    }

    std::string word;
    while (std::cin >> word) {
        program_input[input_lines] = word;
        ++input_lines;
    }

    std::cout << "\n======== [CODE INPUT] ========\n\n";
    output_program_input();

    std::cout << "\n======== [ZLANG OUTPUT] ========\n";
    RUN_ZLANG();

    std::cout << "\n======== [INPUT LINES: " << lines << "]========\n";

    return 0;
}
