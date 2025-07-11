#ifndef DATA_H
#define DATA_H

#include <unordered_map>
#include <string>

extern std::unordered_map<std::string, int> num_vars;

int get_num_val(std::string& name);

#endif
