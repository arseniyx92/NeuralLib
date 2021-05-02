#include "utils/utils.h"
#include "table/table.h"
#include "csv_serializer/csv_serializer.h"
#include "neural_net/neural_net.h"

int main() {
//    Table df = getFromCsv("/home/arseniyx92/Desktop/c++/neural_net_lib/xor.csv");
//    Table X = df.slice(0, df.get_cols()-1, 0, df.get_rows()); // [..)
//    Table Y = df.slice(df.get_cols()-1, df.get_cols(), 0, df.get_rows());
//    Net nt(2, 2, 1, {3});
//    nt.fit(X, Y, 20000);
//    nt.Zip("netZip.txt");
//
//    Net nnt;
//    nnt.UnZip("netZip.txt");
//    for (int i = 0; i < 4; ++i) {
//        Table cur = X.slice(0, X.get_cols(), i, i+1);
//        std::cout << nnt.predict(cur) << std::endl;
//    }
    Table a = getFromCsv("/home/arseniyx92/Desktop/c++/neural_net_lib/training_mush.csv");
    Table X = a.slice(10, 20, 3000, 6000);
    Table Y = a.slice(22, 23, 3000, 6000);
    Net nt(10, 2, 2, {5, 5}, 1);
    nt.fit(X, Y, 2000);
    nt.Zip("netZipped.txt");

    Table Xtest = a.slice(10, 20, 0, a.get_rows());
    Table Ytest = a.slice(a.get_cols()-1, a.get_cols(), 0, a.get_rows());
    Net nnt;
    nnt.UnZip("netZipped.txt");
    std::cout << nnt.accuracy(Xtest, Ytest) << std::endl;
    return 0;
}