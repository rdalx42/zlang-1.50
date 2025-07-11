
#include <iostream>
#include <string>
#include <cmath>
#include <climits>
#include <sstream>
#include "input_parse.h"
#include "data.h"

int operation_statement(std::string x, std::string op, std::string y){

    if(y == ""){
        if(is_num(x)==true){
            return std::stoi(x);
        }else{
            std::cout<<"VALUE ISNT NUM\n";
            return 0;
        }
    }

    if (!x.empty()&&!y.empty()&&op == "+="){
        if(get_num_val(x)!=INT_MAX&&(is_num(y)||get_num_val(y)!=INT_MAX)){
            if(is_num(y)){
                num_vars[x]+=std::stoi(y);
            }else{
                num_vars[x]+=num_vars[y];
            }
        }
    }

    if (!x.empty()&&!y.empty()&&op == "-="){
        if(get_num_val(x)!=INT_MAX&&(is_num(y)||get_num_val(y)!=INT_MAX)){
            if(is_num(y)){
                num_vars[x]-=std::stoi(y);
            }else{
                num_vars[x]-=num_vars[y];
            }
        }
    }

    return LLONG_MAX;
}

void proccess_line(std::string& x){

    std::stringstream ss(x);
    std::string start_keyword,name,eq,op,v,y,z;

    ss>>start_keyword;

    if(start_keyword == "nr"){
        ss>>name>>eq>>v>>y>>z;
        if(name!="" && eq=="="){
            int val = operation_statement(v,y,z);

            if(val && val<INT_MAX && val >= INT_MIN){

                num_vars[name]=val;
            }else{
                std::cout<<"NR VALUE EXCEEDES LIMIT\n";
                return ;
            }
        }else{
            std::cout<<"NR COULDN'T BE DECLARED\n";
            return ;
        }
    }else if(start_keyword == "print") {
        std::string line;
        std::getline(ss, line);

        size_t i = 0;
        while (i < line.size() && isspace(line[i])) {
            ++i;
        }

        for (; i < line.size(); ++i) {
            if (line[i] == '$') {
                std::string varname = "";
                ++i;
                while (i < line.size() && !isspace(line[i])) {
                    varname += line[i];
                    ++i;
                }
                --i;
                if (get_num_val(varname)) {
                    std::cout << num_vars[varname];
                } else {
                    std::cout << varname;
                }
            } else {
                std::cout << line[i];
            }
        }
        std::cout << std::endl;
    }else{
        ss>>op>>z;
        operation_statement(start_keyword,op,z);
    }
}
