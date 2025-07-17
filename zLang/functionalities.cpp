#include <string>
#include <cmath>
#include <climits>
#include <iostream>
#include <sstream>
#include "input_parse.h"
#include "data.h"
#include "statemenets.h"

void proccess_line(std::string& x) {
    line_indx++;
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
    }else if(start_keyword == "if"){
        std::string Z,Y,OpenParem;
        ss >> z >> Z >> Y >> OpenParem;

        // here we have to parse the if statement

        if(bool_statement(z,Z,Y) == false){

            // skip false if

            //int depth = 0;
            //std::string content = "";
            //std::cout<<line_indx<<"\n";
            for(int i = line_indx;i<lines; ++ line_indx){
               // std::cout<<progr
             //   if(zlang_input[i].find("{")!=std::string::npos){depth++;}
              //  if(zlang_input[i].find("}")!=std::string::npos){depth--;}

                //content+=zlang_input[i];
                line_indx=i;
            }std::cout<<line_indx<<"\n";

        } else if (start_keyword == "if") {
            std::string Z, Y, OpenBrace;
            ss >> z >> Z >> Y >> OpenBrace;

            bool condition = bool_statement(z, Z, Y);

            int depth = 0;
            bool started = false;

            for (int i = line_indx; i < lines; ++i) {
                std::string line = zlang_input[i];

                if (line.find("{") != std::string::npos) {
                    depth++;
                    started = true;
                    continue; // Skip the opening brace
                }

                if (line.find("}") != std::string::npos) {
                    depth--;
                    if (depth == 0) {
                        line_indx = i + 1; // Advance to the line after block
                        break;
                    }
                    continue; // Skip the closing brace
                }

                if (started && depth > 0) {
                    if (condition) {
                        proccess_line(line);
                    }
                }
            }
        }

    }else {
        ss >> op >> z;
        std::string Z,Y;
        ss >> Z >> Y;

        if (get_var_type(start_keyword)=="num type"){
            num_vars[start_keyword] = operation_statement(z,Z,Y);
        }

        // only a bool or an if statement can have a bool_statement

        if(get_var_type(start_keyword) == "bool type"){

            bool_vars[start_keyword]=bool_statement(z,Z,Y);

        }
    }

}
