#pragma once

#include "../utils/utils.h"
#include "../table/table.h"

using Layer = std::vector<double>;

class Net {
public:
    Net() = default;
    Net(int input_size, int output_size, int hidden_layers_amount, const std::vector<int>& hidden_layers, int _function_type = 0);

    double function(double x);
    double derivative_function(double x);

    void fit(Table &samp, Table &ans, int global_iterations, int shott = 0);
//    int predict(Table &input); // TODO: make it possible
    int predict(Table input); // if not by link
    double accuracy(Table &Xtest, Table &Ytest);
    void learn(int global_iterations);
    void propagate_front(int cur_layer);
    std::vector<double> propagate_back(int cur_layer, std::vector<double> y, std::vector<std::vector<Layer> >& derivative, std::vector<Layer>& bias_derivative);

    void Zip(const std::string& FileName);
    void UnZip(const std::string& FileName);
private:
    enum FUNCTIONS{
        SIGMOID,
        TANH,
        RELU, // doesn't work
    };
    FUNCTIONS function_type = SIGMOID;
    const double MOD = 100;
    Table samples, answers;
    std::vector<int> shuffler;
    int shot;
    std::vector<Layer> mesh, meshZ;
    std::vector<std::vector<double> > biases, init_biases;
    std::vector<std::vector<std::vector<double> > > weights, init;
};