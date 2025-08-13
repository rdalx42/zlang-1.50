#ifndef DATA_H
#define DATA_H

#include <unordered_map>
#include <string>
#include <vector>

struct var_storage;

extern var_storage GLOBAL_DATA;
extern std::vector<var_storage> program_data;
 extern std::unordered_map<std::string, int> num_vars;
 extern std::unordered_map<std::string, std::string> string_vars;
 extern std::unordered_map<std::string, bool> bool_vars;
 //extern std::unordered_map<std::string, std::vector<std::string>> fn_params;
 //extern std::unordered_map<std::string, std::string> fn_contents;

struct function_read_info{
    std::string name;
    int start_indx;
    int end_indx;
    std::string filename;
};

int get_function_read_info_arr_indx(std::string& fname, const std::string& filename);
int get_num_val(std::string& name, std::string& filename);
std::string get_bool_val(std::string& name, std::string& filename);
std::string get_string_val(std::string& name, std::string& filename);
std::string get_var_type(std::string& name, std::string& filename);
void set_variable_to_data(std::string& varname, std::string& data_name);
void set_variable(std::string& name, std::string& value);
bool is_datatype(std::string& value);
int get_index(const std::string& name);
extern std::vector<function_read_info> function_read_info_arr;

struct var_storage {
    enum TYPES { NUM, BOOL, NULL_TYPE };
    struct var_data {
        std::string name;
        std::string value;
        TYPES type;
        bool constant;
    };
    std::unordered_map<std::string, int> name_to_indx;
    std::vector<var_data> values;
    std::string name;
    std::unordered_map<std::string, std::string> fn_values;
    std::unordered_map<std::string, std::vector<std::string>> fn_params;
    std::unordered_map<std::string, std::string> fn_contents;
    void add_variable(const std::string& name, bool constant, const std::string& TYPE, const std::string& VALUE, bool GLOBAL = false);
    void set_variable(const std::string& name, const std::string& v, bool GLOBAL = false);
};

#endif
