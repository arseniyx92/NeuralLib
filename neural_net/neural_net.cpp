#include "neural_net.h"

Net::Net(int input_size, int output_size, int hidden_layers_amount, const std::vector<int>& hidden_layers) {
    std::mt19937 rnd(75);
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
    for (int i = 1; i < (int)mesh.size()-1; ++i){
        mesh[i].resize(hidden_layers[i - 1]);
        init[i].assign(hidden_layers[i - 1], std::vector<double>(mesh[i-1].size(), 0));
        weights[i].resize(hidden_layers[i - 1], std::vector<double>(mesh[i-1].size()));
        biases[i].resize(hidden_layers[i - 1]);
        init_biases[i].resize(hidden_layers[i - 1]);
        for (int j = 0; j < mesh[i].size(); ++j) {
            biases[i][j] = dist(rnd);
            for (int x = 0; x < (int) mesh[i - 1].size(); ++x)
                weights[i][j][x] = dist(rnd);
        }
    }
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

void Net::fit(Table &samp, Table &ans, int global_iterations, int shott){
    shot = shott;
    std::mt19937 rnd(75);
    if (ans.get_cols() != 1){
        std::cerr << "Output table has more than 1 column\n";
        exit(EXIT_FAILURE);
    }
    if (samp.get_cols() != mesh[0].size()){
        std::cerr << "Incorrect input data\n";
        exit(EXIT_FAILURE);
    }
    if ((int)(ans.get_max(0)-ans.get_min(0) + 1) != (int)mesh.back().size()){
        std::cerr << "Incorrect output data\n";
        exit(EXIT_FAILURE);
    }

    samples = samp;
    answers = ans;
    for (int i = 0; i < samples.get_cols(); ++i)
        samples.divide_column(i, std::max(1.0, samples.get_max(i)));

    // creating shuffler
    shuffler.resize(samples.get_rows());
    for (int i = 0; i < shuffler.size(); ++i)
        shuffler[i] = i;
    std::shuffle(shuffler.begin(), shuffler.end(), rnd);
    if (shot == 0) shot = std::max((int)log((int)shuffler.size())/10, 2);
    learn(global_iterations);
}

void Net::learn(int global_iterations){
    for (int iteration = 1; iteration <= global_iterations; ++iteration) {
        int cur = 0;
        bool ok = true;
        while (cur < shuffler.size()) {
            // get new shuffled data
            std::vector<int> cur_shuffle;
            if (cur + shot > shuffler.size()) {
                for (; cur < shuffler.size(); ++cur)
                    cur_shuffle.push_back(shuffler[cur]);
            } else {
                for (int i = cur; i < cur + shot; ++i)
                    cur_shuffle.push_back(shuffler[i]);
                cur += shot;
            }
            // front propagate each sample
            std::vector<Layer> bias_derivative = init_biases;
            std::vector<std::vector<Layer> > derivative = init;
            for (int i:cur_shuffle) {
                // front propagation
                for (int x = 0; x < mesh[0].size(); ++x)
                    mesh[0][x] = samples.get_value(x, i);
                for (int layer = 1; layer < (int) mesh.size(); ++layer)
                    propagate_front(layer);

                // right answer
                std::vector<double> y(mesh.back().size(), 0);
                y[(int) answers.get_value(0, i)] = 1;

//                // counting cost
//                double cost = 0;
//                for (int op = 0; op < mesh.back().size(); ++op)
//                    cost += (mesh.back()[op] - y[op]) * (mesh.back()[op] - y[op]);
//                std::cout << iteration << " iteration - COST: " << cost << std::endl;

                // back_propagation
                for (int layer = (int) mesh.size() - 1; layer >= 1; --layer)
                    y = propagate_back(layer, y, derivative, bias_derivative);
            }
            // get average sum
            double divisor = 1.0 / (double) cur_shuffle.size();
            for (int layer = 1; layer < (int) mesh.size(); ++layer) {
                mult_matrix_on_constant(derivative[layer], divisor);
                mult_matrix_on_constant(bias_derivative[layer], divisor);
                // updating weights

                derivative[layer] = mult_matrix_on_constant(derivative[layer], -1.0);
                bias_derivative[layer] = mult_matrix_on_constant(bias_derivative[layer], -1.);
                weights[layer] = add_matrix(weights[layer], derivative[layer]);
                biases[layer] = add_matrix(biases[layer], bias_derivative[layer]);
//                threshold(biases[layer], MOD);
//                for (int x = 0; x < mesh[layer].size(); ++x)
//                    threshold(weights[layer][x], MOD);
            }

            // learning results
            if (ok && iteration % 50 == 0) {
                ok = false;
                std::cout << (double)iteration / (double)global_iterations << "% finished" << std::endl;
                for (int j = 0; j < 2; ++j) {
                    int i = cur_shuffle[j];
                    std::vector<double> y(mesh.back().size(), 0);
                    y[(int) answers.get_value(0, i)] = 1;
                    for (int layer = 1; layer < (int) mesh.size(); ++layer)
                        propagate_front(layer);
                    double cost = 0;
                    for (int op = 0; op < mesh.back().size(); ++op)
                        cost += (mesh.back()[op] - y[op]) * (mesh.back()[op] - y[op]);
                    std::cout << iteration << " iteration - COST: " << cost << std::endl;
                }
            }
        }
    }
}

void Net::propagate_front(int cur_layer){
    meshZ[cur_layer] = add_matrix<double>(mult_matrix<double>(weights[cur_layer], mesh[cur_layer-1]), biases[cur_layer]);
    mesh[cur_layer] = meshZ[cur_layer];
    for (double & i : mesh[cur_layer])
        i = sigmoid(i);
}

std::vector<double> Net::propagate_back(int cur_layer, std::vector<double> y, std::vector<std::vector<Layer> >& derivative, std::vector<Layer>& bias_derivative){
    if (cur_layer != mesh.size()-1) y = add_matrix(y, mesh[cur_layer]);
    std::vector<double> y_prev(mesh[cur_layer-1].size());
    for (int i = 0; i < mesh[cur_layer].size(); ++i) {
        Layer lol = mult_matrix_on_constant(mesh[cur_layer-1], (2 * sigmoid_derivative(meshZ[cur_layer][i]) * (mesh[cur_layer][i] - y[i])));
        derivative[cur_layer][i] = add_matrix(derivative[cur_layer][i], lol);
        bias_derivative[cur_layer][i] += (2 * sigmoid_derivative(meshZ[cur_layer][i]) * (mesh[cur_layer][i] - y[i]));
        for (int j = 0; j < mesh[cur_layer-1].size(); ++j)
            y_prev[i] += (2 * weights[cur_layer][i][j] * sigmoid_derivative(meshZ[cur_layer][i]) * (mesh[cur_layer][i] - y[i]));

//        threshold(derivative[cur_layer][i], MOD);
    }
//    threshold(y_prev, 1.0);
//    threshold(bias_derivative[cur_layer], MOD);
    y_prev = mult_matrix_on_constant(y_prev, -1.0);
    return y_prev;
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

int Net::predict(Table input){
    for (int x = 0; x < mesh[0].size(); ++x)
        mesh[0][x] = input.get_value(x, 0);
    for (int layer = 1; layer < (int)mesh.size(); ++layer)
        propagate_front(layer);

    int maxx = 0;
    for (int i = 0; i < mesh.back().size(); ++i)
        if (mesh.back()[maxx] < mesh.back()[i]) maxx = i;
    return maxx;
}

double Net::accuracy(Table &Xtest, Table &Ytest) {
    int tests = Xtest.get_rows();
    double TrueAnswers = 0;
    for (int i = 0; i < tests; ++i){
        std::cout << predict(Xtest.slice(0, Xtest.get_cols(), i, i + 1)) << ' ' << Ytest.get_value(0, i) << '\n';
        if (predict(Xtest.slice(0, Xtest.get_cols(), i, i + 1)) == Ytest.get_value(0, i))
            TrueAnswers++;
    }
    return TrueAnswers / (double)tests;
}