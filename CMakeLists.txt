cmake_minimum_required(VERSION 3.19)
project(neural_net_lib)

set(CMAKE_CXX_STANDARD 17)

add_executable(neural_net_lib utils/utils.cpp utils/utils.h table/table.cpp table/table.h main.cpp csv_serializer/csv_serializer.cpp csv_serializer/csv_serializer.h neural_net/neural_net.cpp neural_net/neural_net.h neural_net/net_serializer.cpp)