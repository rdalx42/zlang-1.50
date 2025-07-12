#include <string>
#include <cmath>
#include <climits>
#include <iostream>
#include <sstream>
#include "input_parse.h"
#include "data.h"
#include "statemenets.h"

void proccess_line(std::string& x) {
    std::stringstream ss(x);
    std::string start_keyword, name, eq, op, v, y, z;

    ss >> start_keyword;

    if (start_keyword == "nr") {
        ss >> name >> eq >> v;
        if (!(ss >> y >> z)) {
            y = "";
            z = "";
        }

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
                return;
            }
        } else {
            std::cout << "NR COULDN'T BE DECLARED\n";
            return;
        }
    } else if (start_keyword == "bool") {
        ss >> name >> eq >> v;

        if (!(ss >> y >> z)) {
            y = "";
            z = "";
        }

        if (is_variable(name)) {
            std::cout << "VARIABLE ALREADY EXISTS";
            return;
        }

        if (!name.empty() && eq == "=") {
            if (v == "true") {
                bool_vars[name] = true;
            } else if (v == "false") {
                bool_vars[name] = false;
            } else if (bool_vars.find(v) != bool_vars.end()) {
                // Assign from another bool variable
                bool_vars[name] = bool_vars[v];
            } else {
                bool_vars[name]=bool_statement(v,y,z);
            }
        } else {
            std::cout << "BOOL COULDN'T BE DECLARED\n";
            return;
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
                while (i < line.size() && !isspace(line[i])) {
                    varname += line[i++];
                }
                if (is_variable(varname)) {
                    if (get_var_type(varname) == "string type") std::cout << string_vars[varname];
                    else if (get_var_type(varname) == "bool type")std::cout<<bool_vars[varname];
                    else if (get_var_type(varname) == "num type") std::cout << num_vars[varname];
                } else {
                    std::cout << '$' << varname;
                }
            } else {
                std::cout << line[i++];
            }
        }
    } else if (start_keyword == "toinput") {
        ss >> z;
        if (is_variable(z)) {
            set_variable(z, program_input[current_zlang_input_line]);
            ++current_zlang_input_line;
        } else {
            std::cout << "COULDNT ASSIGN READ VALUE TO NON EXISTENT VARIABLE\n";
        }
    }else if(start_keyword == "--"){
        return;
    }else {
        ss >> op >> z;
        operation_statement(start_keyword, op, z);

        // only a bool or an if statement can have a bool_statement

        if(get_var_type(start_keyword) == "bool type"){
            std::string Z,Y;
            ss >> Z >> Y;

            bool_vars[start_keyword]=bool_statement(z,Z,Y);

        }
    }
}
