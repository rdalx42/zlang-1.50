#include <iostream>
#include <algorithm>
#include <string>
#include <climits>
#include "statements.h"
#include "input_parse.h"
#include "data.h"

int operation_statement(std::string x, std::string op, std::string y, const std::string& filename)
{
    int val_x = 0;
    int val_y = 0;
    var_storage current = program_data[get_index(filename)];
    if (is_num(x))
        val_x = std::stoi(x);
    else if (current.name_to_indx.find(x) != current.name_to_indx.end() && is_num(current.values[current.name_to_indx[x]].value))
        val_x = std::stoi(current.values[current.name_to_indx[x]].value);
    else
    {
        if (is_fn_value(x, filename))
        {
            x = x.substr(1, x.size());
            if (is_num(current.fn_values[x]))
                val_x = std::stoi(current.fn_values[x]);
            else if (num_vars.find(x) != num_vars.end())
                val_x = num_vars[x];
            else
                return 0;
        }
    }
    if (y.empty())
        return val_x;
    if (is_num(y))
        val_y = std::stoi(y);
    else if (current.name_to_indx.find(y) != current.name_to_indx.end() && is_num(current.values[current.name_to_indx[y]].value))
        val_y = std::stoi(current.values[current.name_to_indx[y]].value);
    else
    {
        if (is_fn_value(y, filename))
        {
            y = y.substr(1, y.size());
            if (is_num(current.fn_values[y]))
                val_y = std::stoi(current.fn_values[y]);
            else if (num_vars.find(y) != num_vars.end())
                val_y = num_vars[y];
            else
                return 0;
        }
    }
    std::string not_const_filename = filename;
    if (op == "+=" && get_num_val(x, not_const_filename) != INT_MAX)
    {
        current.set_variable(x, std::to_string(val_x + val_y), false);
        return std::stoi(current.values[current.name_to_indx[x]].value);
    }
    if (op == "-=" && get_num_val(x, not_const_filename) != INT_MAX)
    {
        current.set_variable(x, std::to_string(val_x - val_y), false);
        return std::stoi(current.values[current.name_to_indx[x]].value);
    }
    if (op == "*=" && get_num_val(x, not_const_filename) != INT_MAX)
    {
        current.set_variable(x, std::to_string(val_x * val_y), false);
        return std::stoi(current.values[current.name_to_indx[x]].value);
    }
    if (op == "/=")
    {
        if (val_y == 0)
        {
            std::cout << "Division by zero error\n";
            return 0;
        }
        current.set_variable(x, std::to_string(val_x / val_y), false);
        return std::stoi(current.values[current.name_to_indx[x]].value);
    }
    if (op == "%=")
    {
        if (val_y == 0)
        {
            std::cout << "Modulo by zero error\n";
            return 0;
        }
        current.set_variable(x, std::to_string(val_x % val_y), false);
        return std::stoi(current.values[current.name_to_indx[x]].value);
    }
    if (op == "&=" && get_num_val(x, not_const_filename) != INT_MAX)
    {
        current.set_variable(x, std::to_string(val_x &= val_y), false);
        return std::stoi(current.values[current.name_to_indx[x]].value);
    }
    if (op == "|=" && get_num_val(x, not_const_filename) != INT_MAX)
    {
        current.set_variable(x, std::to_string(val_x |= val_y), false);
        return std::stoi(current.values[current.name_to_indx[x]].value);
    }
    if (op == "^=" && get_num_val(x, not_const_filename) != INT_MAX)
    {
        current.set_variable(x, std::to_string(val_x ^= val_y), false);
        return std::stoi(current.values[current.name_to_indx[x]].value);
    }
    if (op == "<<=" && get_num_val(x, not_const_filename) != INT_MAX)
    {
        current.set_variable(x, std::to_string(val_x <<= val_y), false);
        return std::stoi(current.values[current.name_to_indx[x]].value);
    }
    if (op == ">>=" && get_num_val(x, not_const_filename) != INT_MAX)
    {
        current.set_variable(x, std::to_string(val_x >>= val_y), false);
        return std::stoi(current.values[current.name_to_indx[x]].value);
    }
    if (op == "+")
        return val_x + val_y;
    if (op == "-")
        return val_x - val_y;
    if (op == "*")
        return val_x * val_y;
    if (op == "/")
    {
        if (val_y == 0)
        {
            std::cout << "Division by zero error\n";
            return 0;
        }
        return val_x / val_y;
    }
    if (op == "%")
    {
        if (val_y == 0)
        {
            std::cout << "Modulo by zero error\n";
            return 0;
        }
        return val_x % val_y;
    }
    if (op == "&")
        return val_x & val_y;
    if (op == "|")
        return val_x | val_y;
    if (op == "^")
        return val_x ^ val_y;
    if (op == "<<")
        return val_x << val_y;
    if (op == ">>")
        return val_x >> val_y;
    return INT_MAX;
}

bool bool_statement(std::string x, std::string op, std::string y, const std::string& filename)
{
    
    int yval = INT_MAX;
    int xval = INT_MAX;
    var_storage current = program_data[get_index(filename)];
    if (is_num(y))
        yval = std::stoi(y);
    else if (current.name_to_indx.find(y) != current.name_to_indx.end() && is_num(current.values[current.name_to_indx[y]].value))
        yval = std::stoi(current.values[current.name_to_indx[y]].value);
    else if (is_fn_value(y, filename))
    {
        std::string cy = y.substr(1, y.size());
        if (current.fn_values.find(cy) != current.fn_values.end())
        {
            std::string val = current.fn_values[cy];
            if (is_num(val))
                yval = std::stoi(val);
            else if (num_vars.find(val) != num_vars.end())
                yval = num_vars[val];
        }
    }
    if (is_num(x))
        xval = std::stoi(x);
    else if (current.name_to_indx.find(x) != current.name_to_indx.end() && is_num(current.values[current.name_to_indx[x]].value))
        xval = std::stoi(current.values[current.name_to_indx[x]].value);
    else if (is_fn_value(x, filename))
    {
        std::string cx = x.substr(1, x.size());
        if (current.fn_values.find(cx) != current.fn_values.end())
        {
            std::string val = current.fn_values[cx];
            if (is_num(val))
                xval = std::stoi(val);
            else if (num_vars.find(val) != num_vars.end())
                xval = num_vars[val];
        }
    }
    if (xval != INT_MAX && yval != INT_MAX)
    {
        if (op == "is")
            return xval == yval;
        if (op == "isnt" || op == "isn't")
            return xval != yval;
        if (op == ">=")
            return xval >= yval;
        if (op == "<=")
            return xval <= yval;
        if (op == ">")
            return xval > yval;
        if (op == "<")
            return xval < yval;
    }
    bool xb = false, yb = false;
    bool x_set = false, y_set = false;
    if (x == "true")
    {
        xb = true;
        x_set = true;
    }
    else if (x == "false")
    {
        xb = false;
        x_set = true;
    }
    else if (current.name_to_indx.find(x) != current.name_to_indx.end() &&
             (current.values[current.name_to_indx[x]].value == "true" || current.values[current.name_to_indx[x]].value == "false"))
    {
        xb = (current.values[current.name_to_indx[x]].value == "true");
        x_set = true;
    }
    else if (is_fn_value(x, filename))
    {
        std::string cx = x.substr(1, x.size());
        if (current.fn_values.find(cx) != current.fn_values.end())
        {
            std::string val = current.fn_values[cx];
            if (val == "true")
            {
                xb = true;
                x_set = true;
            }
            else if (val == "false")
            {
                xb = false;
                x_set = true;
            }
            else if (bool_vars.find(val) != bool_vars.end())
            {
                xb = bool_vars[val];
                x_set = true;
            }
        }
    }
    if (y == "true")
    {
        yb = true;
        y_set = true;
    }
    else if (y == "false")
    {
        yb = false;
        y_set = true;
    }
    else if (current.name_to_indx.find(y) != current.name_to_indx.end() &&
             (current.values[current.name_to_indx[y]].value == "true" || current.values[current.name_to_indx[y]].value == "false"))
    {
        yb = (current.values[current.name_to_indx[y]].value == "true");
        y_set = true;
    }
    else if (is_fn_value(y, filename))
    {
        std::string cy = y.substr(1, y.size());
        if (current.fn_values.find(cy) != current.fn_values.end())
        {
            std::string val = current.fn_values[cy];
            if (val == "true")
            {
                yb = true;
                y_set = true;
            }
            else if (val == "false")
            {
                yb = false;
                y_set = true;
            }
            else if (current.name_to_indx.find(val) != current.name_to_indx.end() &&
                    (current.values[current.name_to_indx[val]].value == "true" || current.values[current.name_to_indx[val]].value == "false"))
            {
                yb = (current.values[current.name_to_indx[val]].value == "true");
                y_set = true;
            }
        }
    }
    if (x_set && y_set)
    {
        if (op == "is")
            return xb == yb;
        if (op == "isnt" || op == "isn't")
            return xb != yb;
    }
    if (x_set && !y_set)
        return xb;
    return false;
}
