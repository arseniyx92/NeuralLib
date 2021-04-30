#pragma once

#include "../utils/utils.h"

class Table {
public:
    enum types{
        UNDEFINED,
        STRING,
        INT,
        FLOAT,
    };
    Table();
    Table(const std::vector<std::string> &input_columns);
    Table(const Table &t);
    ~Table() = default;

    void define_column_type(int cnt, types tp);

    void push_new_value_in_column(int cnt, std::string value) {
        switch (std::get<1>(columns[cnt])) {
            case INT:
                int_cols[std::get<2>(columns[cnt])].second.push_back(std::stoi(value));
                if (int_cols[std::get<2>(columns[cnt])].second.size() > rows) rows++;
                break;
            case FLOAT:
                float_cols[std::get<2>(columns[cnt])].second.push_back(std::stod(value));
                if (float_cols[std::get<2>(columns[cnt])].second.size() > rows) rows++;
                break;
            case STRING:
                string_cols[std::get<2>(columns[cnt])].second.push_back(value);
                if (string_cols[std::get<2>(columns[cnt])].second.size() > rows) rows++;
                break;
            default:
                std::cerr << "push_new_value_in_column\n";
                exit(EXIT_FAILURE);
        }
    }

    Table slice(int l_col, int r_col, int l_row, int r_row);

    int get_cols();
    int get_rows();
    double get_max(int col); // WORKS ONLY WITH DOUBLE
    double get_min(int col); // WORKS ONLY WITH DOUBLE
    void divide_column(int col, double div); // WORKS ONLY WITH DOUBLE
    double get_value(int col, int row); // WORKS ONLY WITH DOUBLE
private:
    int cols, rows;
    std::vector<std::tuple<std::string, types, int> > columns;
    std::vector<std::pair<int, std::vector<long long> > > int_cols;
    std::vector<std::pair<int, std::vector<double> > > float_cols;
    std::vector<std::pair<int, std::vector<std::string> > > string_cols;
};