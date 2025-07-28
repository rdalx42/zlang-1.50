#ifndef DATA_H
#define DATA_H

#include <unordered_map>
#include <string>
#include <vector>

extern std::unordered_map<std::string, int> num_vars;
extern std::unordered_map<std::string, std::string> string_vars;
extern std::unordered_map<std::string, bool> bool_vars;
extern std::unordered_map<std::string,std::string>fn_contents;
extern std::unordered_map<std::string, std::vector<std::string>> fn_params;

int get_num_val(std::string& name);

std::string get_bool_val(std::string& name);
std::string get_string_val(std::string& name);
std::string get_var_type(std::string& name);

bool is_variable(std::string& name);
void set_variable(std::string& name, std::string& value);
void set_variable_to_data(std::string& name, std::string& data);

#endif
