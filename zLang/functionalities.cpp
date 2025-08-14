#include <string>
#include <sstream>
#include <iostream>
#include <climits>
#include <chrono>
#include <fstream>
#include <thread>
#include <cstdlib>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "data.h"
#include "input_parse.h"
#include "statements.h"
#include "functionalities.h"

#pragma GCC optimize("O3", "unroll-loops", "fast-math")

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
std::string current_fn_name = "";

void set_console_color(int fg, int bg)
{
    if (bg == -1)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        bg = (csbi.wAttributes & 0xF0) >> 4;
    }
    SetConsoleTextAttribute(hConsole, (bg << 4) | (fg & 0x0F));
}

void handle_nr(std::stringstream& ss, const std::string& filename,  bool is_const=false)
{
    std::string name, eq, expr, extra;
    
    ss >> name >> eq >> expr;
    

    std::getline(ss, extra);
    if (program_data[get_index(filename)].name_to_indx.find(name) != program_data[get_index(filename)].name_to_indx.end())
        return;
    if (!name.empty() && eq == "=")
    {
        int result = operation_statement(expr, "", "", filename);
        if (result < INT_MAX && result >= INT_MIN)
        {
            program_data[get_index(filename)].add_variable(name, is_const, "NUM", std::to_string(result));
           // std::cout<<"setting value!\n";
        }    
        else
        {
            std::cout << "NR VALUE EXCEEDS LIMIT\n";
        }    
    }
    else
    {
        std::cout << "NR COULDN'T BE DECLARED\n";
    }    
}

void handle_bool(std::stringstream& ss, const std::string& filename, bool is_const)
{
    std::string name, eq, expr, extra, extra2;
    ss >> name >> eq >> expr >> extra >> extra2;
    
    if (program_data[get_index(filename)].name_to_indx.find(name) != program_data[get_index(filename)].name_to_indx.end())
        return;
    if (!name.empty() && eq == "=")
    {
        bool result = false;
        if (expr == "true"&&extra.empty() && extra2.empty())
            result = true;
        else if (expr == "false"&&extra.empty() && extra2.empty())
            result = false;
        else
            result = bool_statement(expr, extra, extra2, filename);
        program_data[get_index(filename)].add_variable(name, is_const, "bool type", result ? "true" : "false");
    }
    else
        std::cout << "BOOL COULDN'T BE DECLARED\n";
}

void handle_print(std::stringstream& ss, const std::string& filename)
{
    std::string line;
    std::getline(ss, line);
    
    size_t i = 0;
    size_t j = line.size();

    bool got_start=false;
    bool got_end=false;

    if(line.empty())
    {
        std::cout<<"Cannot print empty content\n";
        return;
    }

    while(i<line.size()){
        if(line[i] == '\"' || line[i] == '\''){
            got_start=true;
            break;
        }
        ++i;
    }

    while (j>i){
        if(line[j] == '\"'||line[j] == '\''){
            got_end=true;
            break;
        }
        --j;
    }

    if(got_end==false || got_start==false){
        std::cout<<"Expected quotes at end and start of print statement\n";
        return;
    }

    ++i; 
    --j; 
 
    while (i <= j && isspace(line[i]))
        ++i;
    while (i <= j)
    {
        if (i + 1 <= j && line[i] == '\\' && std::tolower(line[i + 1]) == 'n')
        {
            std::cout << "\n";
            i += 2;
        }
        else if (line[i] == '$')
        {
            std::string varname;
            ++i;
            while (i <= j  && !isspace(line[i]))
                varname += line[i++];
            if (program_data[get_index(filename)].name_to_indx.find(varname) != program_data[get_index(filename)].name_to_indx.end())
            {
                std::string filename2 = filename;
                std::string type = get_var_type(varname, filename2);
                std::string val;
                if (type == "num type")
                    val = std::to_string(get_num_val(varname, filename2));
                else if (type == "bool type")
                    val = get_bool_val(varname, filename2);
                else
                    val = get_string_val(varname, filename2);
                std::cout << val;
            }
            else
                std::cout << '$' << varname;
        }
        else
            std::cout << line[i++];
    }
}

void skip_if_block(int& line_indx, const std::string& filename, int lines)
{
    int depth = 0;
    for (int i = line_indx; i < lines; ++i)
    {
        const std::string& line = zlang_input[i];
        if (line.find("{") != std::string::npos)
            depth++;
        if (line.find("}") != std::string::npos)
        {
            depth--;
            if (depth == 0)
            {
                line_indx = i + 1;
                return;
            }
        }
    }
}

void handle_if(std::stringstream& ss, int& line_indx, const std::string& filename, int lines)
{
    std::string left, op, right, brace;
    ss >> left >> op >> right >> brace;

    bool condition = bool_statement(left, op, right, filename);

    int depth = 0;
    int startBlock = -1;
    int endBlock = -1;

    for (int i = line_indx; i < lines; ++i)
    {
        if (zlang_input[i].find("{") != std::string::npos)
        {
            if (depth == 0) startBlock = i + 1;
            depth++;
        }
        else if (zlang_input[i].find("}") != std::string::npos)
        {
            depth--;
            if (depth == 0)
            {
                endBlock = i;
                break;
            }
        }
    }

    if (startBlock == -1 || endBlock == -1)
    {
        std::cout << "Syntax error: if block missing braces\n";
        return;
    }

    if (condition)
    {
        for (int i = startBlock; i < endBlock; ++i)
        {
            proccess_line(zlang_input[i], i, filename,lines);
        }
    }

    line_indx = endBlock + 1;
}



void handle_while(std::stringstream& ss, int& line_indx, const std::string& filename, int lines)
{
    std::string left, op, right, brace;
    ss >> left;
    std::string token;
    if (!(ss >> token))
    {
        std::cout << "Syntax error: expected condition or '{' after while\n";
        return;
    }
    if (token == "{")
    {
        op = "";
        right = "";
        brace = "{";
    }
    else
    {
        op = token;
        if (!(ss >> right))
        {
            std::cout << "Syntax error: expected right operand or '{'\n";
            return;
        }
        if (!(ss >> brace) || brace != "{")
        {
            std::cout << "Syntax error: expected '{' after condition\n";
            return;
        }
    }
    int start = line_indx + 1;
    int end = -1;
    int depth = 0;
    for (int i = start; i < lines; ++i)
    {
        const std::string& line = zlang_input[i];
        if (line.find("{") != std::string::npos)
            depth++;
        if (line.find("}") != std::string::npos)
        {
            if (depth == 0)
            {
                end = i;
                break;
            }
            depth--;
        }
    }
    if (end == -1)
    {
        std::cout << "Syntax error: matching '}' not found for while\n";
        return;
    }
    while ( op.empty() ? bool_statement(left, "is", "true", filename) : bool_statement(left, op, right, filename) )
    {
        for (int i = start; i < end; ++i)
            proccess_line(zlang_input[i], i, filename,lines);
    }
    line_indx = end + 1;
}

void handle_function_def(std::stringstream& ss, int& line_indx, const std::string& filename, int lines)
{
    std::string name, token;
    ss >> name >> token;
    current_fn_name = name;
    if (token == ":")
    {
        std::string param;
        while (ss >> param && param != "{")
        {
            program_data[get_index(filename)].fn_params[name].push_back(param);
            program_data[get_index(filename)].add_variable(param, false, "NUM", "0");
        }
    }

    std::string not_const_filename = filename;

    function_read_info info{name, line_indx, 0, not_const_filename};
    std::string fn_body;
    int depth = 0;
    bool started = false;
    for (int i = line_indx; i < lines; ++i)
    {
        std::string& line = zlang_input[i];
        if (line.find("{") != std::string::npos)
        {
            depth++;
            started = true;
            if (depth == 1)
                continue;
        }
        if (line.find("}") != std::string::npos && started)
        {
            depth--;
            if (depth == 0)
            {
                info.end_indx = i + 1;
                function_read_info_arr.push_back(info);
                program_data[get_index(filename)].fn_contents[name] = fn_body;
                line_indx = i + 1;
                return;
            }
        }
        if (started && depth > 0)
            fn_body += line + "\n";
    }
}


void handle_function_fire(std::stringstream& ss, const std::string& filename, int lines)
{
    std::string fn_name, token;
    ss >> fn_name >> token;

    int fn_index = -1;
    for (int i = 0; i < function_read_info_arr.size(); ++i)
    {
        if (function_read_info_arr[i].name == fn_name && function_read_info_arr[i].filename == filename)
        {
            fn_index = i;
            break;
        }
    }

    if (fn_index == -1)
    {
        std::cout << "FUNCTION '" << fn_name << "' NOT FOUND IN FILE '" << filename << "'\n";
        return;
    }

    current_fn_name = fn_name;

    program_data[get_index(filename)].fn_values[fn_name] = "";

    auto& fn_params = program_data[get_index(filename)].fn_params;
    if (!fn_params[fn_name].empty() && token == ":")
    {
        int idx = 0;
        std::string arg;
        while (idx < fn_params[fn_name].size() && ss >> arg)
        {
            set_variable_to_data(fn_params[fn_name][idx++], arg);
        }
        if (idx < fn_params[fn_name].size())
        {
            std::cout << "Not enough parameters for function: " << fn_name << "\n";
            current_fn_name = "";
            return;
        }
    }

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << filename << "\n";
        current_fn_name = "";
        return;
    }

    std::vector<std::string> file_lines;
    std::string line;
    while (std::getline(file, line))
        file_lines.push_back(line);
    file.close();

    int start = function_read_info_arr[fn_index].start_indx + 1;
    int end = function_read_info_arr[fn_index].end_indx - 1;
    if (start < 0) start = 0;
    if (end > static_cast<int>(file_lines.size())) end = file_lines.size();

    for (int i = start; i < end; ++i)
    {
        remove_whitespace(file_lines[i]);

        proccess_line(file_lines[i], i, filename,lines);

        if (!program_data[get_index(filename)].fn_values[fn_name].empty())
        {
            break;
        }
    }
    if (program_data[get_index(filename)].fn_values[fn_name].empty())
        program_data[get_index(filename)].fn_values[fn_name] = "";

    current_fn_name = "";
}

void handle_import(const std::string& filename, const std::string& current_filename){
    
    std::ifstream file(filename);
    
    if(filename == current_filename){
        std::cout<<"Cannot import the same file as the current one!\n";
        file.close();
        return;
    }

    if(filename.substr(filename.size()-3) != ".zl"){
        std::cout<<"Only zLang files can be imported!\n";
        file.close();
        return;
    }

    if(!file.is_open()){
        std::cout<<"Couldn't import file "<<filename<<"\n";
        file.close();
        return;
    }

    RUN_ZLANG(filename, get_input_from_file(filename), get_size_from_file(filename));

    file.close();
}

void waitms(int ms)
{
    if (ms < 0 || ms > 3600000)
        std::cout << "WAITMS TIME OUT OF RANGE (0–3600000)\n";
    else
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void waits(int s)
{
    if (s < 0 || s > 3600)
        std::cout << "WAIT TIME OUT OF RANGE (0–3600 seconds)\n";
    else
        std::this_thread::sleep_for(std::chrono::milliseconds(s * 1000));
}

void proccess_line(std::string& x, int& line_indx, const std::string& filename, int lines)
{
    remove_whitespace(x);
    if (x.empty())
        return;
    std::stringstream ss(x);
    std::string token;
    ss >> token;
    if (token == "num")
        handle_nr(ss, filename,false );
    else if(token == "const")
    {
        std::string token2;
        ss>>token2;

        if(token2 == "num"){
            handle_nr(ss,filename,true);
        }else if(token2 == "bool"){
            handle_bool(ss,filename,true);
        }else{
            std::cout<<"CANNOT CREATE CONSTANT INSTANCE OF: "<<token2<<"\n";
        }
    }
    else if(token == "clear" || token == "cls"){
        system("cls");
    }
    else if (token == "bool")
        handle_bool(ss, filename,false);
    else if (token == "print")
        handle_print(ss, filename);
    
    else if (token == "toinput")
    {
        std::string var;
        ss >> var;
        if (program_data[get_index(filename)].name_to_indx.find(var) == program_data[get_index(filename)].name_to_indx.end())
            set_variable_to_data(var, program_input[current_zlang_input_line++]);
        else
            std::cout << "COULD NOT ASSIGN TO NON-EXISTENT VARIABLE\n";
    }
    else if (token == "--")
        return;
    else if (token == "if"){
       // std::cout<<"if\n";
        handle_if(ss, line_indx, filename,lines);
    }else if (token == "while"){
        handle_while(ss, line_indx, filename,lines);
    }else if (token == "fn"){
        handle_function_def(ss, line_indx, filename, lines);
    }else if (token == "return")
    {
        std::string value;
        std::string filename_non_const = filename;
        ss >> value;
        if (current_fn_name.empty())
            std::cout << "Return not allowed outside function\n";
        else if (value.empty())
            std::cout << "Return value cannot be empty!\n";
        else if (!is_datatype(value,filename_non_const))
            std::cout << "Invalid return datatype!\n";
        else if (program_data[get_index(filename)].fn_values[current_fn_name].empty())
            if(!is_num(value)&&!is_bool(value))
            {
                value=program_data[get_index(filename)].values[program_data[get_index(filename)].name_to_indx[value]].value;
            }
                
            program_data[get_index(filename)].fn_values[current_fn_name] = value;
    }
    else if (token == "fire")
        handle_function_fire(ss, filename,lines);
    else if (token == "waitms")
    {
        int ms;
        ss >> ms;
        waitms(ms);
    }else if (token == "import") {
        std::string rest_of_line;
        std::getline(ss, rest_of_line); 

        auto trim = [](std::string& s) {
            s.erase(0, s.find_first_not_of(" \t"));
            s.erase(s.find_last_not_of(" \t") + 1);
        };
        trim(rest_of_line);

        if (rest_of_line.size() >= 2 && 
            rest_of_line.front() == '"' && 
            rest_of_line.back() == '"') {

            std::string import_filename = rest_of_line.substr(1, rest_of_line.size() - 2);
            handle_import(import_filename, filename);
        } 
        else {
            std::cerr << "Imported filename must be in quotes\n";
        }
    }

    else if (token == "wait")
    {
        int s;
        ss >> s;
        waits(s);
    }else if (token == "color")
    {
        int fg = -1, bg = -1;
        ss >> fg;
        if (ss >> bg)
        {
            set_console_color(fg, bg);
        }
        else if (fg != -1)
        {
            set_console_color(fg, -1); // only foreground, keep current background
        }
        else
        {
            std::cout << "Usage: color <fg> [bg]\n";
        }
    }

    else
    {
        std::string op, arg1, arg2, arg3;
        ss >> op >> arg1 >> arg2 >> arg3;
        int prog_idx = get_index(filename);
        if (prog_idx == -1)
        {
            std::cout << "File not found in program_data\n";
            return;
        }

        std::string filename_non_const = filename;

        std::string type = get_var_type(token, filename_non_const);
        if (type == "num type")
        {
            int result = operation_statement(arg1, arg2, arg3, filename);
            auto it = program_data[prog_idx].name_to_indx.find(token);
            if (it != program_data[prog_idx].name_to_indx.end() && program_data[prog_idx].values[it->second].constant==false)
                program_data[prog_idx].values[it->second].value = std::to_string(result);
            else
                std::cout << "Can't modify variable: " << token << " - variable is either constant or null\n";
        }
        else if (type == "bool type")
        {
            bool result = bool_statement(arg1, arg2, arg3, filename);
            auto it = program_data[prog_idx].name_to_indx.find(token);
            if (it != program_data[prog_idx].name_to_indx.end() && program_data[prog_idx].values[it->second].constant==false)
                program_data[prog_idx].values[it->second].value = result ? "true" : "false";
            else
                std::cout << "Can't modify variable: " << token << " - variable is either constant or null\n";
        }
        else
        {
            std::cout << "Unsupported variable type: " << token << "\n";
        }    
    }
}
