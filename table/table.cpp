#include "table.h"

Table::Table() {
    cols = 0;
    rows = 0;
}

Table::Table(const std::vector<std::string> &input_columns) {
    cols = (int) input_columns.size();
    rows = 0;
    columns.resize(cols);
    for (int i = 0; i < cols; ++i) {
        std::get<0>(columns[i]) = input_columns[i];
        std::get<1>(columns[i]) = UNDEFINED;
    }
}

Table::Table(const Table &t){
    cols = t.cols;
    rows = t.rows;
    columns = t.columns;
    int_cols = t.int_cols;
    float_cols = t.float_cols;
    string_cols = t.string_cols;
}

void Table::define_column_type(int cnt, types tp){
    std::get<1>(columns[cnt]) = tp;
    switch (tp) {
        case INT:
            std::get<2>(columns[cnt]) = int_cols.size();
            int_cols.push_back({cnt, {}});
            break;
        case FLOAT:
            std::get<2>(columns[cnt]) = float_cols.size();
            float_cols.push_back({cnt, {}});
            break;
        case STRING:
            std::get<2>(columns[cnt]) = string_cols.size();
            string_cols.push_back({cnt, {}});
            break;
        default:
            std::cerr << "Trying to define_column_type to UNDEFINED\n";
            exit(EXIT_FAILURE);
    }
}

Table Table::slice(int l_col, int r_col, int l_row, int r_row){
    std::vector<std::string> new_columns;
    for (int i = l_col; i < r_col; ++i)
        new_columns.push_back(std::get<0>(columns[i]));
    Table t(new_columns);
    for (int i = l_col; i < r_col; ++i){
        t.define_column_type(i-l_col, std::get<1>(columns[i]));
        int cnt = std::get<2>(columns[i]);
        switch (std::get<1>(columns[i])) {
            case INT:
                for (int j = l_row; j < r_row; ++j)
                    t.push_new_value_in_column(i-l_col, std::to_string(int_cols[cnt].second[j]));
                break;
            case FLOAT:
                for (int j = l_row; j < r_row; ++j)
                    t.push_new_value_in_column(i-l_col, std::to_string(float_cols[cnt].second[j]));
                break;
            case STRING:
                for (int j = l_row; j < r_row; ++j)
                    t.push_new_value_in_column(i-l_col, string_cols[cnt].second[j]);
                break;
            default:
                std::cerr << "slice\n";
                exit(EXIT_FAILURE);
        }
    }
    return t;
}

int Table::get_cols(){
    return cols;
}

int Table::get_rows(){
    return rows;
}

double Table::get_max(int col){
    double ans = 0;
    for (double& i:float_cols[std::get<2>(columns[col])].second)
        ans = std::max(ans, i);
    return ans;
}

double Table::get_min(int col){
    double ans = 1e9+7;
    for (double& i:float_cols[std::get<2>(columns[col])].second)
        ans = std::min(ans, i);
    return ans;
}

void Table::divide_column(int col, double div){
    for (double &i:float_cols[std::get<2>(columns[col])].second)
        i /= div;
}

double Table::get_value(int col, int row){
    return float_cols[std::get<2>(columns[col])].second[row];
}