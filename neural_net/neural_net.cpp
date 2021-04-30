#include "neural_net.h"

std::mt19937 rnd(time(NULL));

Net::Net(int input_size, int output_size, int hidden_layers_amount, const std::vector<int>& hidden_layers) {
    if (input_size < 2){
        std::cerr << "Minimum input size is 2\n";
        exit(EXIT_FAILURE);
    }
    mesh.resize(2 + hidden_layers_amount);
    init.resize(2 + hidden_layers_amount);
    weights.resize(2 + hidden_layers_amount);
    init_biases.resize(2 + hidden_layers_amount);
    biases.resize(2 + hidden_layers_amount);
    mesh[0].resize(input_size);
    mesh.back().resize(output_size);

    auto dist = std::uniform_real_distribution<double>(0.0, 1.0);
    for (int i = 1; i < (int) mesh.size(); ++i){
        mesh[i].resize(hidden_layers[i - 1]);
        init[i].assign(hidden_layers[i - 1], std::vector<double>(mesh[i-1].size(), 0));
        weights[i].resize(hidden_layers[i - 1], std::vector<double>(mesh[i-1].size()));
        biases.resize(hidden_layers[i - 1]);
        init_biases.resize(hidden_layers[i - 1]);
        for (int j = 0; j < mesh[i].size(); ++j) {
            biases[i][j] = dist(rnd);
            for (int x = 0; x < (int) mesh[i - 1].size(); ++x)
                weights[i][j][x] = dist(rnd);
        }
    }
    rnd = std::mt19937(time(NULL));
    weights.back().resize(output_size, std::vector<double>(mesh[mesh.size()-2].size()));
    init.back().assign(output_size, std::vector<double>(mesh[mesh.size()-2].size(), 0));
    biases.back().resize(output_size);
    init_biases.back().resize(output_size);
    for (int j = 0; j < mesh.back().size(); ++j) {
        biases.back()[j] = dist(rnd);
        for (int x = 0; x < (int) mesh[mesh.size() - 2].size(); ++x)
            weights.back()[j][x] = dist(rnd);
    }
    meshZ = mesh;
}

void Net::fit(Table &samp, Table &ans){
    if (ans.get_cols() != 1){
        std::cerr << "Output table has more than 1 column\n";
        exit(EXIT_FAILURE);
    }
    if (samp.get_cols() != mesh[0].size()){
        std::cerr << "Incorrect input data\n";
        exit(EXIT_FAILURE);
    }
    if ((int)(ans.get_max(0)-ans.get_min(0)) != (int)mesh.back().size()){
        std::cerr << "Incorrect output data\n";
        exit(EXIT_FAILURE);
    }

    samples = samp;
    answers = ans;
    for (int i = 0; i < samples.get_cols(); ++i)
        samples.divide_column(i, samples.get_max(i));

    // creating shuffler
    shuffler.resize(samples.get_rows());
    for (int i = 0; i < shuffler.size(); ++i)
        shuffler[i] = i;
    std::shuffle(shuffler.begin(), shuffler.end(), rnd());
    shot = std::max((int)log((int)shuffler.size()), 2);
    learn();
}

void Net::learn(){
    int cur = 0;
    while (cur < shuffler.size()){
        // get new shuffled data
        std::vector<int> cur_shuffle;
        if (cur + shot > shuffler.size()){
            for (; cur < shuffler.size(); ++cur)
                cur_shuffle.push_back(shuffler[cur]);
        }else{
            for (int i = cur; i < cur+shot; ++i)
                cur_shuffle.push_back(shuffler[i]);
            cur += shot;
        }
        // front propagate each sample
        std::vector<std::vector<Layer> > derivative = init;
        for (int i:cur_shuffle){
            // front propagation
            for (int x = 0; x < mesh[0].size(); ++x)
                mesh[0][x] = samples.get_value(x, i);
            for (int layer = 1; layer < (int)mesh.size(); ++layer)
                propagate_front(layer);

            // back_propagation
        }
    }
    // calculate error (cost)
    // add cost to sum
    // get average sum
}

void Net::propagate_front(int cur_layer){
    meshZ[cur_layer] = add_matrix<double>(mult_matrix<double>(weights[cur_layer], mesh[cur_layer-1]), biases[cur_layer]);
    mesh[cur_layer] = meshZ[cur_layer];
    for (double & i : mesh[cur_layer])
        i = sigmoid(i);
}

std::vector<double> Net::propagate_back(int cur_layer, std::vector<double> y, std::vector<std::vector<Layer> >& derivative){
    for (int i = 0; i < mesh[cur_layer].size(); ++i) {
        Layer lol = mult_matrix_on_constant(mesh[cur_layer], (2 * sigmoid_derivative(meshZ[cur_layer][i]) *
                                                              (mesh[cur_layer][i] - y[i])));
        derivative[cur_layer][i] = add_matrix(derivative[cur_layer][i], lol);
    }
}

int Net::predict(Table &input){
    for (int x = 0; x < mesh[0].size(); ++x)
        mesh[0][x] = input.get_value(x, 0);
    for (int layer = 1; layer < (int)mesh.size(); ++layer)
        propagate_front(layer);

    int maxx = 0;
    for (int i = 0; i < mesh.back().size(); ++i)
        if (mesh.back()[maxx] < mesh.back()[i]) maxx = i;
    return maxx;
}