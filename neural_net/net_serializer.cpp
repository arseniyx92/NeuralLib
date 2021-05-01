#include "neural_net.h"
#include "../utils/utils.h"

template <typename T, typename EnableIfType = void>
struct net_serializer{
    static void serialize(std::ostream& out, T data){
        data.serialize(out);
    }
    static T deserialize(std::istream& in){
        T data;
        data.deserialize(in);
        return data;
    }
};

//template <>
//struct net_serializer<int>{
//    static void serialize(std::ostream& out, int value){
//        out << (unsigned char)(value) << (unsigned char)(value >> 8)
//          << (unsigned char)(value >> 16) << (unsigned char)(value >> 24);
//    }
//    static int deserialize(std::istream& in){
//        unsigned char current_byte;
//        int value = 0;
//        for (int i = 0; i < 4; ++i){
//            current_byte = in.get();
//            value |= ( ((int)current_byte) << (8 * i) );
//        }
//        return value;
//    }
//};

template<typename T>
struct net_serializer<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
    static void serialize(std::ostream &s, T value) {
        std::uint8_t buf[sizeof(T)];
        std::memcpy(buf, &value, sizeof(T));
        for (std::size_t i = 0; i < sizeof(T); ++i) {
            s << buf[i];
        }
    }
    static T deserialize(std::istream &is) {
        std::uint8_t buf[sizeof(T)];
        for (std::size_t i = 0; i < sizeof(T); ++i) {
            buf[i] = is.get();
        }
        T result;
        std::memcpy(&result, buf, sizeof(T));
        return result;
    }
};

template <typename T>
struct net_serializer<std::vector<T> >{
    static void serialize(std::ostream& out, const std::vector<T>& data){
        net_serializer<int>::serialize(out, data.size());
        for (const auto& i:data)
            net_serializer<T>::serialize(out, i);
    }
    static std::vector<T> deserialize(std::istream& in){
        int size = net_serializer<int>::deserialize(in);
        std::vector<T> data(size);
        for (auto& i:data)
            i = net_serializer<T>::deserialize(in);
        return data;
    }
};

template <typename T>
void saveToFile(const std::string& FileName, T data){
    std::ofstream out(FileName, std::ios::binary);
    if (!out.is_open()){
        std::cerr << "File "+FileName+" can't be written to\n";
        exit(EXIT_FAILURE);
    }
    net_serializer<T>::serialize(out, data);
}

template <typename T>
T getFromFile(const std::string& FileName){
    std::ifstream in(FileName, std::ios::binary);
    if (!in.is_open()){
        std::cerr << "File "+FileName+" can't be listened from\n";
        exit(EXIT_FAILURE);
    }
    return net_serializer<T>::deserialize(in);
}

void Net::Zip(const std::string& FileName){
    std::ofstream out(FileName, std::ios::binary);
    if (!out.is_open()){
        std::cerr << "File "+FileName+" can't be written to\n";
        exit(EXIT_FAILURE);
    }
    net_serializer<int>::serialize(out, (int)mesh[0].size());
    net_serializer<decltype(biases)>::serialize(out, biases);
    net_serializer<decltype(weights)>::serialize(out, weights);
}

void Net::UnZip(const std::string& FileName){
    std::ifstream in(FileName, std::ios::binary);
    if (!in.is_open()){
        std::cerr << "File "+FileName+" can't be written to\n";
        exit(EXIT_FAILURE);
    }
    int input = net_serializer<int>::deserialize(in);
    biases = net_serializer<decltype(biases)>::deserialize(in);
    weights = net_serializer<decltype(weights)>::deserialize(in);

    init.resize(weights.size());
    mesh.resize(biases.size());
    init_biases.resize(biases.size());
    for (int i = 0; i < (int)biases.size(); ++i){
        if (i == 0) {
            init[i].resize(0);
            mesh[i].assign(input, 0);
            init_biases[i].assign(biases.size(), 0);
        }else {
            init[i].assign(weights[i].size(), Layer(weights[i][0].size(), 0));
            mesh[i].assign(biases[i].size(), 0);
            init_biases[i].assign(biases.size(), 0);
        }
    }
    meshZ = mesh;
}