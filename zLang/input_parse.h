#ifndef INPUT_PARSE_H
#define INPUT_PARSE_H

#include <string>

#define MAXDIM 1000

extern std::string zlang_input[MAXDIM];
extern int lines;

void remove_whitespace(std::string& x);
void validate_input();
void output_input();
bool is_num(std::string& x);

#endif // INPUT_PARSE_H
