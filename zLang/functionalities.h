#ifndef FUNCTIONALITIES_H
#define FUNCTIONALITIES_H

#include <string>

void manage_statement();
void proccess_line(std::string& x, int& indx,const std::string& filename, int lines);
void set_console_color(int fg, int bg);

#endif
