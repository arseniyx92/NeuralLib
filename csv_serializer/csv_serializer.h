#pragma once

#include "../utils/utils.h"
#include "../table/table.h"

//template <typename T> struct csv_serializer {
//    static void serialize(std::ostream &out, T value) {
//        value.serialize(out);
//    }
//
//    static T deserialize(std::istream in) {
//        T x;
//        x.deserialize(in);
//        return x;
//    }
//};
//
//template <> struct csv_serializer<int> {
//    static void serialize(std::ostream &out, int value) {
//        out << value;
//    }
//
//    static int deserialize(std::istream in) {
//        int x;
//        in >> x;
//        return x;
//    }
//};
//
//template <> struct csv_serializer<std::string> {
//    static void serialize(std::ostream &out, std::string value) {
//        out << value;
//    }
//
//    static std::string deserialize(std::istream in) {
//        std::string x;
//        in >> x;
//        return x;
//    }
//};

void saveCsv(const std::string &fileName, Table data);

Table getFromCsv(const std::string &fileName);