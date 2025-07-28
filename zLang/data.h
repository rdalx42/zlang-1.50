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
extern std::unordered_map<std::string, std::string> fn_values;

struct function_read_info{
    std::string name;
    int start_indx = 0;
    int end_indx = 0;
};

extern std::vector<function_read_info>function_read_info_arr;

int get_num_val(std::string& name);

std::string get_bool_val(std::string& name);
std::string get_string_val(std::string& name);
std::string get_var_type(std::string& name);

bool is_variable(std::string& name);
bool is_datatype(std::string& value);
void set_variable(std::string& name, std::string& value);
void set_variable_to_data(std::string& name, std::string& data);
int get_function_read_info_arr_indx(std::string& fname);

#endif
