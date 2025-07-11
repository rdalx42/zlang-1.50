#include <string>
#include <cmath>
#include <climits>
#include <iostream>
#include <sstream>
#include "input_parse.h"
#include "data.h"

int operation_statement(std::string x, std::string op, std::string y) {
    int val_x = 0;
    int val_y = 0;

    if (is_num(x)) {
        val_x = std::stoi(x);
    } else if (get_num_val(x) != INT_MAX) {
        val_x = num_vars[x];
    } else {
        return 0;
    }

    if (y.empty()) {
        return val_x;
    }

    if (is_num(y)) {
        val_y = std::stoi(y);
    } else if (get_num_val(y) != INT_MAX) {
        val_y = num_vars[y];
    } else {
        return 0;
    }

    if (op == "+=" && get_num_val(x) != INT_MAX) {
        num_vars[x] += val_y;
        return num_vars[x];
    }
    if (op == "-=" && get_num_val(x) != INT_MAX) {
        num_vars[x] -= val_y;
        return num_vars[x];
    }
    if (op == "*=" && get_num_val(x) != INT_MAX) {
        num_vars[x] *= val_y;
        return num_vars[x];
    }
    if (op == "/=") {
        if (val_y == 0) {
            std::cout << "Division by zero error\n";
            return 0;
        }
        if (get_num_val(x) != INT_MAX) {
            num_vars[x] /= val_y;
            return num_vars[x];
        }
    }
    if (op == "%=") {
        if (val_y == 0) {
            std::cout << "Modulo by zero error\n";
            return 0;
        }
        if (get_num_val(x) != INT_MAX) {
            num_vars[x] %= val_y;
            return num_vars[x];
        }
    }
    if (op == "&=" && get_num_val(x) != INT_MAX) {
        num_vars[x] &= val_y;
        return num_vars[x];
    }
    if (op == "|=" && get_num_val(x) != INT_MAX) {
        num_vars[x] |= val_y;
        return num_vars[x];
    }
    if (op == "^=" && get_num_val(x) != INT_MAX) {
        num_vars[x] ^= val_y;
        return num_vars[x];
    }
    if (op == "<<=" && get_num_val(x) != INT_MAX) {
        num_vars[x] <<= val_y;
        return num_vars[x];
    }
    if (op == ">>=" && get_num_val(x) != INT_MAX) {
        num_vars[x] >>= val_y;
        return num_vars[x];
    }
    if (op == "+") return val_x + val_y;
    if (op == "-") return val_x - val_y;
    if (op == "*") return val_x * val_y;
    if (op == "/") {
        if (val_y == 0) {
            std::cout << "Division by zero error\n";
            return 0;
        }
        return val_x / val_y;
    }
    if (op == "%") {
        if (val_y == 0) {
            std::cout << "Modulo by zero error\n";
            return 0;
        }
        return val_x % val_y;
    }
    if (op == "&") return val_x & val_y;
    if (op == "|") return val_x | val_y;
    if (op == "^") return val_x ^ val_y;
    if (op == "<<") return val_x << val_y;
    if (op == ">>") return val_x >> val_y;

    return LLONG_MAX;
}

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
                if (get_num_val(varname) != INT_MAX) {
                    std::cout << num_vars[varname];
                } else {
                    std::cout << varname;
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
    } else {
        ss >> op >> z;
        operation_statement(start_keyword, op, z);
    }
}
