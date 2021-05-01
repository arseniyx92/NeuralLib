#pragma once

#include "../utils/utils.h"
#include "../table/table.h"

using Layer = std::vector<double>;

class Net {
public:
    Net() = default;
    Net(int input_size, int output_size, int hidden_layers_amount, const std::vector<int>& hidden_layers);

    void fit(Table &samp, Table &ans, int global_iterations);
    int predict(Table &input);
    void learn(int global_iterations);
    void propagate_front(int cur_layer);
    std::vector<double> propagate_back(int cur_layer, std::vector<double> y, std::vector<std::vector<Layer> >& derivative, std::vector<Layer>& bias_derivative);
private:
    Table samples, answers;
    std::vector<int> shuffler;
    int shot;
    std::vector<Layer> mesh, meshZ;
    std::vector<std::vector<double> > biases, init_biases;
    std::vector<std::vector<std::vector<double> > > weights, init;
};