#ifndef INPUT_PARSE_H
#define INPUT_PARSE_H

#include <string>
#include <vector>
#include "data.h"

#define MAXDIM 10000

extern std::string zlang_input[MAXDIM];
extern std::string program_input[MAXDIM];

extern int lines;
extern int input_lines;
extern int current_zlang_input_line;
extern int var_indx;

void RUN_ZLANG(const std::string& x);
void clean_program_input();
void remove_whitespace(std::string& x);
void output_input();
void output_program_input();

bool is_bool(std::string& x);
bool is_num(std::string& x);
bool is_fn_value(std::string& x, const std::string& filename);

#endif
