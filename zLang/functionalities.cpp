#include <string>
#include <cmath>
#include <climits>
#include <iostream>
#include <sstream>
#include "input_parse.h"
#include "data.h"
#include "statemenets.h"
#include "functionalities.h"
#include <fstream>
#include <chrono>
#include <thread>

void proccess_line(std::string& x, int& line_indx) {
    std::stringstream ss(x);
    std::string start_keyword;
    ss >> start_keyword;

    if (start_keyword == "nr") {
        std::string name, eq, v, y, z;
        ss >> name >> eq >> v;
        if (!(ss >> y >> z)) y = z = "";

        if (is_variable(name)) {
            std::cout << "VARIABLE ALREADY EXISTS";
            return;
        }

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

    } else if (start_keyword == "bool") {
        std::string name, eq, v, y, z;
        ss >> name >> eq >> v;
        if (!(ss >> y >> z)) y = z = "";

        if (is_variable(name)) {
            std::cout << "VARIABLE ALREADY EXISTS";
            return;
        }

        if (!name.empty() && eq == "=") {
            if (v == "true") bool_vars[name] = true;
            else if (v == "false") bool_vars[name] = false;
            else if (bool_vars.find(v) != bool_vars.end()) bool_vars[name] = bool_vars[v];
            else bool_vars[name] = bool_statement(v, y, z);
        } else {
            std::cout << "BOOL COULDN'T BE DECLARED\n";
        }

    } else if (start_keyword == "print") {
        std::string line;
        std::getline(ss, line);

        size_t i = 0;
        while (i < line.size() && isspace(line[i])) ++i;

        while (i < line.size()) {
            if ((i + 1) < line.size() && line[i] == '!' && std::tolower(line[i + 1]) == 'n') {
                std::cout << "\n";
                i += 2;
                continue;
            }

            if (line[i] == '$') {
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

    } else if (start_keyword == "toinput") {
        std::string var;
        ss >> var;
        if (is_variable(var)) {
            set_variable(var, program_input[current_zlang_input_line]);
            ++current_zlang_input_line;
        } else {
            std::cout << "COULDNT ASSIGN READ VALUE TO NON EXISTENT VARIABLE\n";
        }

    } else if (start_keyword == "--") {
        return;

    } else if (start_keyword == "if") {
        std::string z, op, y, brace;
        ss >> z >> op >> y >> brace;
        bool condition = bool_statement(z, op, y);
        int depth = 0;
        bool found_block = false;

        if (!condition) {
            for (int i = line_indx; i < lines; ++i) {
                std::string line = zlang_input[i];
                if (line.find("{") != std::string::npos) {
                    depth++;
                    found_block = true;
                }
                if (line.find("}") != std::string::npos && found_block) {
                    depth--;
                    if (depth == 0) {
                        line_indx = i + 1;
                        return;
                    }
                }
            }
            return;
        } else {
            for (int i = line_indx; i < lines; ++i) {
                std::string line = zlang_input[i];
                if (line.find("{") != std::string::npos) {
                    depth++;
                    found_block = true;
                    continue;
                }
                if (line.find("}") != std::string::npos && found_block) {
                    depth--;
                    if (depth == 0) {
                        line_indx = i + 1;
                        return;
                    }
                    continue;
                }
                if (found_block && depth > 0) {
                    proccess_line(line, i);
                }
            }
        }

    } else if (start_keyword == "fn") {
        std::string name, open;
        ss >> name >> open;

        if (open == ":") {

            std::string param;
            while (ss >> param) {

                if(param == "{"){break;}
                fn_params[name].push_back(param);

            }
        }

        std::string fn_content = "";
        int depth = 0;
        bool started = false;

        for (int i = line_indx ; i < lines; ++i) {
            std::string line = zlang_input[i];

            if (line.find("{") != std::string::npos) {
                depth++;
                started = true;
                continue;
            }

            if (line.find("}") != std::string::npos && started) {
                depth--;
                if (depth == 0) {
                    line_indx = i + 1;
                    break;
                }
                continue;
            }

            if (started && depth > 0) {
                fn_content += line + "\n";
            }
        }

        fn_contents[name] = fn_content;

    } else if (start_keyword == "fire") {

        std::string fn_name,param_keyword;
        ss >> fn_name>>param_keyword;

        if (fn_params[fn_name].size() > 0 && param_keyword == ":") {
            std::string param_value;
            int param_indx = 0;

            while (param_indx < (int)fn_params[fn_name].size() && ss >> param_value) {
                std::string& param_name = fn_params[fn_name][param_indx];
                set_variable_to_data(param_name, param_value);
                param_indx++;
            }

            if (param_indx < (int)fn_params[fn_name].size()) {
                std::cout << "Not enough params when firing function of name: " << fn_name << "\n";
                return;
            }
        }

        if (fn_contents.find(fn_name) != fn_contents.end()) {
            std::stringstream fn_stream(fn_contents[fn_name]);
            std::string line;
            int dummy_line = 0;
            while (std::getline(fn_stream, line)) {
                proccess_line(line, dummy_line);
            }
        } else {
            std::cout << "FUNCTION '" << fn_name << "' NOT FOUND\n";
        }

    }else if(start_keyword == "clear"){
        system("cls");
    }else if(start_keyword == "run"){
        std::string filename;
        ss>>filename;

        if (filename.size() < 4 || filename.substr(filename.size() - 3) != ".zl") {
            std::cout << "ONLY FILES WITH .zl EXTENSION CAN BE RUN\n";
            return;
        }

        std::ifstream runfile(filename);

        if (!runfile) {
            std::cout << "FAILED TO OPEN FILE: " << filename << "\n";
            return;
        }

        std::string temp_zlang_input[MAXDIM];
        int temp_lines = 0;

        std::string line;
        while (std::getline(runfile, line)) {
            temp_zlang_input[temp_lines++] = line;
        }

        for (int i = 0; i < temp_lines; ++i) {
            remove_whitespace(temp_zlang_input[i]);
            proccess_line(temp_zlang_input[i], i);
        }
    } else if (start_keyword == "wait") {
        std::string x;
        ss >> x;

        int wait_time = 0;

        if (is_num(x)) {
            wait_time = std::stoi(x);
        } else if (num_vars.find(x) != num_vars.end()) {
            wait_time = num_vars[x];
        } else {
            std::cout << "INVALID WAIT TIME OR VARIABLE NOT FOUND\n";
            return;
        }

        if (wait_time < 0 || wait_time > 3600) {
            std::cout << "WAIT TIME OUT OF RANGE (0-3600)\n";
            return;
        }

        std::this_thread::sleep_for(std::chrono::seconds(wait_time));

    }else {
        std::string op, z, Z, Y;
        ss >> op >> z >> Z >> Y;

        if (get_var_type(start_keyword) == "num type") {
            num_vars[start_keyword] = operation_statement(z, Z, Y);
        } else if (get_var_type(start_keyword) == "bool type") {
            bool_vars[start_keyword] = bool_statement(z, Z, Y);
        }
    }
}
