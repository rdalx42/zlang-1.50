#include <iostream>
#include <string>
#include "input_parse.h"
#include "functionalities.h"
#include "data.h"
#include <vector>

std::string zlang_input[MAXDIM];
std::string program_input[MAXDIM];

int lines = 0;
int input_lines = 0;
int current_zlang_input_line = 0;
int var_indx = 0;



std::vector<std::string> space_tokens = {
    "=",
    "+",
    "-",
    "/",
    "*",
    ":",
    "{",
    "}",
    "--"
};

void remove_whitespace(std::string& x) {
    size_t i = 0;
    while (i < x.size() && x[i] == ' ') ++i;
    x = x.substr(i);
}

void RUN_ZLANG(const std::string& filename, std::vector<std::string> source, int lines_of_code) {
    
    // here's the main issue with the zlang run function when importing 
    // probably overwritten or smth, look into here
    
    zLangProgram CURRENT_DATA;
    CURRENT_DATA.name = filename;
    CURRENT_DATA.file_input.resize(lines_of_code);
    
    for (int i = 0; i < lines_of_code; ++i) {
        
        std::string line = source[i];  
        remove_whitespace(line);
        CURRENT_DATA.file_input[i]=line;
       // std::cout<<CURRENT_DATA.file_input[i]<<"\n";
    }

    program_data[var_indx]=CURRENT_DATA;
    
    std::cout<<CURRENT_DATA.name<<"\n";

    for(int i = 0; i < lines_of_code; ++ i){
        program_data[get_index(filename)].line_indx=i;
        
        proccess_line(program_data[get_index(filename)].file_input[i],i,filename,lines_of_code);
    }

    var_indx++;
}

std::vector<std::string> get_input_from_file(const std::string& filename) {

    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cout << "Couldn't read code input from non existing file\n";
        return {"0"};
    }
    
    std::vector<std::string> input_lines;
    std::string ln; 

    while (std::getline(file, ln)) {
        input_lines.push_back(ln);
    }

    return input_lines;
}

int get_size_from_file(const std::string& filename){
    
    std::ifstream file(filename);
    
    if(!file.is_open()){
        std::cout << "Couldn't read code input from non existing file\n";
        return 0;
    }

    std::string ln; 
    int line_count = 0;

    while (std::getline(file, ln)) {
        line_count ++ ;
    }

    return line_count;
}

void clean_program_input(const std::string& filename) {
    for (int l = 0; l < lines; ++l) {
        std::string current_ln = program_data[get_index(filename)].file_input[l];
        std::string cleaned_line;
        bool in_string = false;
        char string_delim = '\0';

        for (size_t i = 0; i < current_ln.size(); ++i) {
            char c = current_ln[i];
            if (c == ',') c = ' ';
            if ((c == '"' || c == '\'') && (i == 0 || current_ln[i - 1] != '\\')) {
                if (!in_string) {
                    if (!cleaned_line.empty() && cleaned_line.back() != ' ') cleaned_line += ' ';
                    cleaned_line += c;
                    in_string = true;
                    string_delim = c;
                    if (i + 1 < current_ln.size() && current_ln[i + 1] != ' ') cleaned_line += ' ';
                } else if (c == string_delim) {
                    if (!cleaned_line.empty() && cleaned_line.back() != ' ') cleaned_line += ' ';
                    cleaned_line += c;
                    in_string = false;
                    string_delim = '\0';
                    if (i + 1 < current_ln.size() && current_ln[i + 1] != ' ') cleaned_line += ' ';
                }
                continue;
            }

            if (in_string) {
                cleaned_line += c;
                continue;
            }

            bool matched_token = false;
            for (const std::string& token : space_tokens) {
                size_t token_len = token.length();
                if (current_ln.compare(i, token_len, token) == 0) {
                    if (!cleaned_line.empty() && cleaned_line.back() != ' ') cleaned_line += ' ';
                    if (token == "--") {
                        cleaned_line += "- -";
                    } else {
                        cleaned_line += token;
                    }
                    if (i + token_len < current_ln.size() && current_ln[i + token_len] != ' ') cleaned_line += ' ';
                    i += token_len - 1;
                    matched_token = true;
                    break;
                }
            }

            if (!matched_token) cleaned_line += c;
        }

        program_data[get_index(filename)].file_input[l] = cleaned_line;
    }
}

void output_input(const std::string& filename) {
    for (int i = 0; i < lines; ++i) std::cout << program_data[get_index(filename)].file_input[i] << "\n";
}

void output_program_input() {
    for (int i = 0; i < input_lines; ++i) {
        std::cout << "LINE: " << i + 1 << ": " << program_input[i] << "\n";
        std::cout << "--------";
        for (int j = 0; j < program_input[i].size(); ++j) std::cout << "-";
        std::cout << "\n";
    }
}

bool is_num(std::string& x) {
    try {
        size_t pos;
        std::stoi(x, &pos);
        return pos == x.size();
    } catch (...) {
        return false;
    }
}

bool is_bool(std::string& x) {
    return x == "true" || x == "false";
}

bool is_fn_value(std::string& x, const std::string& filename) {
    size_t colonpos = x.find(":");
    if (colonpos != std::string::npos) {
        std::string fn_name = x.substr(1, x.size());
        if (get_function_read_info_arr_indx(fn_name,filename) != -1) return true;
    }
    return false;
}
