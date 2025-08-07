#include <string>
#include <sstream>
#include <iostream>
#include <climits>
#include <chrono>
#include <thread>
#include <windows.h>
#include "input_parse.h"
#include "data.h"
#include "statemenets.h"
#include "functionalities.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
std::string current_fn_name = "";

void set_console_color(int fg, int bg = -1) {
    if (bg == -1) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        bg = (csbi.wAttributes & 0xF0) >> 4;
    }
    SetConsoleTextAttribute(hConsole, (bg << 4) | (fg & 0x0F));
}

void handle_nr(std::stringstream& ss) {
    std::string name, eq, v, y = "", z = "";
    ss >> name >> eq >> v >> y >> z;

    if (is_variable(name)) return;
    if (!name.empty() && eq == "=") {
        int val = y.empty() ? operation_statement(v, "", "") : operation_statement(v, y, z);
        if (val < INT_MAX && val >= INT_MIN) {
            num_vars[name] = val;
        } else {
            std::cout << "NR VALUE EXCEEDS LIMIT\n";
        }
    } else {
        std::cout << "NR COULDN'T BE DECLARED\n";
    }
}

void handle_bool(std::stringstream& ss) {
    std::string name, eq, v, y = "", z = "";
    ss >> name >> eq >> v >> y >> z;

    if (is_variable(name)) {
        std::cout << "VARIABLE ALREADY EXISTS\n";
        return;
    }

    if (!name.empty() && eq == "=") {
        if (v == "true") bool_vars[name] = true;
        else if (v == "false") bool_vars[name] = false;
        else if (bool_vars.count(v)) bool_vars[name] = bool_vars[v];
        else bool_vars[name] = bool_statement(v, y, z);
    } else {
        std::cout << "BOOL COULDN'T BE DECLARED\n";
    }
}

void handle_print(std::stringstream& ss) {
    std::string line;
    std::getline(ss, line);

    size_t i = 0;
    while (i < line.size() && isspace(line[i])) ++i;

    while (i < line.size()) {
        if (i + 1 < line.size() && line[i] == '!' && std::tolower(line[i + 1]) == 'n') {
            std::cout << "\n";
            i += 2;
        } else if (line[i] == '$') {
            std::string varname;
            ++i;
            while (i < line.size() && !isspace(line[i])) varname += line[i++];
            if (is_variable(varname)) {
                std::string type = get_var_type(varname);
                if (type == "string type") std::cout << string_vars[varname];
                else if (type == "bool type") std::cout << bool_vars[varname];
                else if (type == "num type") std::cout << num_vars[varname];
            } else {
                std::cout << '$' << varname;
            }
        } else {
            std::cout << line[i++];
        }
    }
}

void skip_if_block(int& line_indx) {
    int depth = 0;
    bool found = false;
    for (int i = line_indx; i < lines; ++i) {
        const std::string& line = zlang_input[i];
        if (line.find("{") != std::string::npos) { depth++; found = true; }
        if (line.find("}") != std::string::npos && found) {
            depth--;
            if (depth == 0) {
                line_indx = i + 1;
                return;
            }
        }
    }
}

void handle_if(std::stringstream& ss, int& line_indx) {
    std::string left, op, right, brace;
    ss >> left >> op >> right >> brace;

    bool condition = bool_statement(left, op, right);

    if (!condition) {
        skip_if_block(line_indx);
    } else {
        int depth = 0;
        bool inside_block = false;

        for (int i = line_indx; i < lines; ++i) {
            std::string& line = zlang_input[i];
            if (line.find("{") != std::string::npos) {
                depth++; inside_block = true;
                continue;
            }
            if (line.find("}") != std::string::npos && inside_block) {
                depth--;
                if (depth == 0) {
                    line_indx = i + 1;
                    return;
                }
                continue;
            }
            if (inside_block && depth > 0) {
                proccess_line(line, i);
            }
        }
    }
}

void handle_function_def(std::stringstream& ss, int& line_indx) {
    std::string name, token;
    ss >> name >> token;
    current_fn_name = name;

    if (token == ":") {
        std::string param;
        while (ss >> param && param != "{") {
            fn_params[name].push_back(param);
        }
    }

    function_read_info info{name, line_indx, 0};
    std::string fn_body;
    int depth = 0;
    bool started = false;

    for (int i = line_indx; i < lines; ++i) {
        std::string& line = zlang_input[i];
        if (line.find("{") != std::string::npos) {
            depth++; started = true;
            if (depth == 1) continue;
        }
        if (line.find("}") != std::string::npos && started) {
            depth--;
            if (depth == 0) {
                info.end_indx = i + 1;
                function_read_info_arr.push_back(info);
                fn_contents[name] = fn_body;
                line_indx = i + 1;
                return;
            }
        }
        if (started && depth > 0) {
            fn_body += line + "\n";
        }
    }
}

void handle_function_fire(std::stringstream& ss) {
    fn_values[current_fn_name] = "";

    std::string fn_name, param_token;
    ss >> fn_name >> param_token;
    int fn_index = get_function_read_info_arr_indx(fn_name);

    if (fn_params[fn_name].size() > 0 && param_token == ":") {
        int param_idx = 0;
        std::string param_val;
        while (param_idx < fn_params[fn_name].size() && ss >> param_val) {
            set_variable_to_data(fn_params[fn_name][param_idx++], param_val);
        }

        if (param_idx < fn_params[fn_name].size()) {
            std::cout << "Not enough params for function: " << fn_name << "\n";
            return;
        }
    }

    if (!fn_contents.count(fn_name)) {
        std::cout << "FUNCTION '" << fn_name << "' NOT FOUND\n";
        return;
    }

    current_fn_name = fn_name;
    for (int i = function_read_info_arr[fn_index].start_indx + 1;
         i < function_read_info_arr[fn_index].end_indx - 1; ++i) {
        remove_whitespace(zlang_input[i]);
        proccess_line(zlang_input[i], i);
    }
    current_fn_name = "";
}

void proccess_line(std::string& x, int& line_indx) {
    std::stringstream ss(x);
    std::string keyword;
    ss >> keyword;

    if (keyword == "nr") handle_nr(ss);
    else if (keyword == "bool") handle_bool(ss);
    else if (keyword == "print") handle_print(ss);
    else if (keyword == "toinput") {
        std::string var;
        ss >> var;
        if (is_variable(var)) {
            set_variable_to_data(var, program_input[current_zlang_input_line++]);
        } else {
            std::cout << "COULD NOT ASSIGN TO NON-EXISTENT VARIABLE\n";
        }
    } else if (keyword == "--") {
        return;
    } else if (keyword == "if") {
        handle_if(ss, line_indx);
    } else if (keyword == "fn") {
        handle_function_def(ss, line_indx);
    } else if (keyword == "return") {
        std::string value;
        ss >> value;
        if (current_fn_name.empty()) {
            std::cout << "Cannot return outside of function\n";
        } else if (value.empty()) {
            std::cout << "Return value must not be empty!\n";
        } else if (!is_datatype(value)) {
            std::cout << "Cannot return non-existent data type value!\n";
        } else if (fn_values[current_fn_name].empty()) {
            fn_values[current_fn_name] = value;
        }
    } else if (keyword == "fire") {
        handle_function_fire(ss);
    } else if (keyword == "clear") {
        system("cls");
    } else if (keyword == "wait") {
        std::string time_str;
        ss >> time_str;
        int wait_time = is_num(time_str) ? std::stoi(time_str) : num_vars[time_str];
        if (wait_time < 0 || wait_time > 3600) {
            std::cout << "WAIT TIME OUT OF RANGE (0–3600)\n";
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(wait_time));
        }
    } else if (keyword == "color") {
        std::string mode;
        int fg = 7, bg = -1;
        ss >> mode;
        if (mode == "fg") ss >> fg;
        else if (mode == "bg") ss >> bg;
        else if (mode == "both") ss >> fg >> bg;
        else std::cout << "INVALID COLOR MODE. USE: fg, bg, or both\n";
        set_console_color(fg, bg);
    } else {
        std::string op, z, Z, Y;
        ss >> op >> z >> Z >> Y;
        std::string type = get_var_type(keyword);
        if (type == "num type") num_vars[keyword] = operation_statement(z, Z, Y);
        else if (type == "bool type") bool_vars[keyword] = bool_statement(z, Z, Y);
    }
}
