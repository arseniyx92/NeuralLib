#pragma once

// INCLUDES
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <tuple>
#include <random>
#include <ctime>
#include <algorithm>
#include <cstring>
#include <thread>
#include <mutex>

// FUNCTIONS
inline double sigmoid(const double& x){
    if (1.0 + std::exp(-x) == 0) return 0;
    return 1.0 / (1.0 + std::exp(-x));
}

inline double sigmoid_derivative(const double& x){
    double sigm = sigmoid(x);
    return sigm*(1-sigm);
}

inline double hyp_tan(const double& x){
    return tanh(x);
}

inline double hyp_tan_derivative(const double& x){
    return 1 - tanh(x) * tanh(x);
}

inline double ReLU(const double& x){
    if (x <= 0) return 0;
    return x;
}

inline double ReLU_derivative(const double& x){
    if (x <= 0) return 0;
    return 1;
}

template <typename T>
std::vector<std::vector<T> > mult_matrix(const std::vector<std::vector<T> >& a, std::vector<std::vector<T> > b){
    int n = a.size();
    int m = b.size();
    int k = b[0].size();
    std::vector<std::vector<T> > ans(n, std::vector<T>(k));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < k; ++j)
            for (int x = 0; x < m; ++x)
                ans[i][j] += a[i][x]*b[x][j];
    return ans;
}

template <typename T>
std::vector<T> mult_matrix(const std::vector<std::vector<T> >& a, std::vector<T> b){
    int n = a.size();
    int m = b.size();
    std::vector<T> ans(n);
    for (int i = 0; i < n; ++i)
            for (int x = 0; x < m; ++x)
                ans[i] += a[i][x]*b[x];
    return ans;
}

template <typename T>
std::vector<std::vector<T> > add_matrix(const std::vector<std::vector<T> >& a, const std::vector<std::vector<T> >& b){
    int n = a.size();
    int m = a[0].size();
    std::vector<std::vector<T> > ans(n, std::vector<T>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            ans[i][j] = a[i][j] + b[i][j];
    return ans;
}

template <typename T>
std::vector<T> add_matrix(const std::vector<T>& a, const std::vector<T>& b){
    int n = a.size();
    std::vector<T> ans(n);
    for (int i = 0; i < n; ++i)
        ans[i] = a[i] + b[i];
    return ans;
}

template <typename T>
std::vector<std::vector<T> > mult_matrix_on_constant(const std::vector<std::vector<T> >& a, const T& C){
    int n = a.size();
    int m = a[0].size();
    std::vector<std::vector<T> > ans(n, std::vector<T>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            ans[i][j] = a[i][j]*C;
    return ans;
}

template <typename T>
std::vector<T> mult_matrix_on_constant(const std::vector<T>& a, const T& C){
    int n = a.size();
    std::vector<T> ans(n);
    for (int i = 0; i < n; ++i)
        ans[i] = a[i]*C;
    return ans;
}

template <typename T>
void threshold(std::vector<T>& vec, const T& MAXX){
    for (int i = 0; i < (int)vec.size(); ++i)
        if (abs(vec[i]) > MAXX)
            vec[i] = (vec[i]/abs(vec[i]))*MAXX;
}