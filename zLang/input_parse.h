#ifndef INPUT_PARSE_H
#define INPUT_PARSE_H

#include <string>

#define MAXDIM 10000

extern std::string zlang_input[MAXDIM];
extern std::string program_input[MAXDIM];

extern int lines;
extern int input_lines;
extern int current_zlang_input_line;

void RUN_ZLANG();
void remove_whitespace(std::string& x);
void output_input();
void output_program_input();

bool is_num(std::string& x);

#endif // INPUT_PARSE_H
