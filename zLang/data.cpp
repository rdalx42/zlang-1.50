#include <string>
#include <cmath>
#include <climits>
#include <iostream>
#include <unordered_map>
#include "data.h"
#include "input_parse.h"

zLangProgram GLOBAL_DATA;
std::vector<zLangProgram> program_data;
std::unordered_map<std::string, int> num_vars;
std::unordered_map<std::string, std::string> string_vars;
std::unordered_map<std::string, bool> bool_vars;
std::vector<function_read_info> function_read_info_arr;
//std::unordered_map<std::string, std::string> fn_contents;
//std::unordered_map<std::string, std::vector<std::string>> fn_params;

int get_index(const std::string& name)
{
    for (int i = 0; i < program_data.size(); i++)
    {
        if (program_data[i].name == name){
          //  std::cout<<program_data[i].name<<"\n";
            return i;
        }    
    }
    return -1;
}

int get_num_val(std::string& name, std::string& filename)
{
    int idx = get_index(filename);
    if (idx == -1)
        return 0;
    zLangProgram &vs = program_data[idx];
    if (vs.name_to_indx.find(name) != vs.name_to_indx.end())
        return std::stoi(vs.values[vs.name_to_indx[name]].value);
    return 0;
}

std::string get_bool_val(std::string& name, std::string& filename)
{
    int idx = get_index(filename);
    if (idx == -1)
        return "";
    zLangProgram &vs = program_data[idx];
    if (vs.name_to_indx.find(name) != vs.name_to_indx.end())
        return vs.values[vs.name_to_indx[name]].value;
    return "";
}

std::string get_string_val(std::string& name, std::string& filename)
{
    int idx = get_index(filename);
    if (idx == -1)
        return "";
    zLangProgram &vs = program_data[idx];
    if (vs.name_to_indx.find(name) != vs.name_to_indx.end())
        return vs.values[vs.name_to_indx[name]].value;
    return "";
}

std::string get_var_type(std::string& name, std::string& filename)
{
    int idx = get_index(filename);
    if (idx == -1)
        return "";
    zLangProgram &vs = program_data[idx];
    if (vs.name_to_indx.find(name) != vs.name_to_indx.end())
    {
        zLangProgram::var_data &vd = vs.values[vs.name_to_indx[name]];
        if (vd.type == zLangProgram::NUM)
            return "num type";
        if (vd.type == zLangProgram::BOOL)
            return "bool type";
        return "unknown";
    }
    return "";
}

int get_function_read_info_arr_indx(std::string& fname, const std::string& filename)
{
    for (int i = 0; i < program_data[get_index(filename)].function_read_info_arr.size(); i++)
    {
        if ( program_data[get_index(filename)].function_read_info_arr[i].name == fname &&  program_data[get_index(filename)].function_read_info_arr[i].filename == filename)
            return i;
    }
    return -1;
}

void set_variable_to_data(std::string& varname, std::string& data_name)
{
    if (!program_data.empty())
        program_data[0].set_variable(varname, data_name);
}

bool is_datatype(std::string& value, std::string& filename) // this might cause some bugs.
{
    if (value == "NUM" || value == "bool type" || value == "BOOL" || is_num(value) || is_bool(value) || program_data[get_index(filename)].name_to_indx.find(value) != program_data[get_index(filename)].name_to_indx.end())
        return true;
    return false;
}

void zLangProgram::add_variable(const std::string& name, bool constant, const std::string& TYPE, const std::string& VALUE, bool GLOBAL)
{
    if (name_to_indx.find(name) != name_to_indx.end())
        return;
    var_data data;
    data.name = name;
    data.value = VALUE;
    if (TYPE == "NUM")
        data.type = NUM;
    else if (TYPE == "bool type" || TYPE == "BOOL")
        data.type = BOOL;
    else
        data.type = NULL_TYPE;
    data.constant = constant;
    name_to_indx[name] = values.size();
    values.push_back(data);
}

void zLangProgram::set_variable(const std::string& name, const std::string& v, bool GLOBAL)
{

    if (name_to_indx.find(name) == name_to_indx.end())
    {
        return;
    }

    if (values[name_to_indx[name]].constant==true)
    {
       // std::cout<<"ERROR; CAN'T MODIFY CONSTANT VARIABLE: "<<name<<"\n";
        return;
    }
    
    values[name_to_indx[name]].value = v;
}
