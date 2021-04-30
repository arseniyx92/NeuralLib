#include "csv_serializer.h"

Table getFromCsv(const std::string &fileName) {
    std::fstream in(fileName, std::ios::in);
    if (!in.is_open()){
        std::cerr << "File "+fileName+" can't be opened\n";
        exit(EXIT_FAILURE);
    }
    std::string str, cur;
    std::vector<std::string> columns;
    std::getline(in, str);
    str += ',';
    for (char ch:str){
        if (ch == ','){
            columns.push_back(cur);
            cur = "";
        }else
            cur += ch;
    }
    Table t(columns);
    bool is_defined = false;
    while (std::getline(in, str)){
        str += ',';
        if (!is_defined){
            int cnt = 0;
            for (char ch:str){
                if (ch == ' ') continue;
                if (ch == ','){
                    if (isdigit(cur[0])){
                        t.define_column_type(cnt, t.FLOAT);
                        t.push_new_value_in_column(cnt, cur);
                    }else{
                        t.define_column_type(cnt, t.STRING);
                        t.push_new_value_in_column(cnt, cur);
                    }
                    cur = "";
                    cnt++;
                }else
                    cur += ch;
            }
            is_defined = true;
            continue;
        }
        int cnt = 0;
        for (char ch:str){
            if (ch == ' ') continue;
            if (ch == ','){
                if (isdigit(cur[0])){
                    t.push_new_value_in_column(cnt, cur);
                }else{
                    t.push_new_value_in_column(cnt, cur);
                }
                cur = "";
                cnt++;
            }else
                cur += ch;
        }
    }
    return t;
}