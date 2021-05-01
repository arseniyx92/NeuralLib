#include "utils/utils.h"
#include "table/table.h"
#include "csv_serializer/csv_serializer.h"
#include "neural_net/neural_net.h"

int main() {
//    Table a = getFromCsv("/home/arseniyx92/Desktop/c++/neural_net_lib/training_mush.csv");
//    Table b = a.slice(3, 5, 2, 5);

//    Table lol = getFromCsv("/home/arseniyx92/Desktop/c++/neural_net_lib/xor.csv");
//    Table X = lol.slice(0, 2, 0, 4);
//    Table Y = lol.slice(2, 3, 0, 4);
//    Net nt(2, 2, 2, {2, 2});
//    nt.fit(X, Y, 2000);
//
//    Table test = lol.slice(0, 2, 0, 1);
//    std::cout << nt.predict(test) << std::endl;
//    test = lol.slice(0, 2, 1, 2);
//    std::cout << nt.predict(test) << std::endl;
//    test = lol.slice(0, 2, 2, 3);
//    std::cout << nt.predict(test) << std::endl;
//    test = lol.slice(0, 2, 3, 4);
//    std::cout << nt.predict(test) << std::endl;
//
//
//    std::cout << std::endl;
//    nt.Zip("netZipped.txt");
//    Net nnt;
//    nnt.UnZip("netZipped.txt");
//    test = lol.slice(0, 2, 0, 1);
//    std::cout << nnt.predict(test) << std::endl;
//    test = lol.slice(0, 2, 1, 2);
//    std::cout << nnt.predict(test) << std::endl;
//    test = lol.slice(0, 2, 2, 3);
//    std::cout << nnt.predict(test) << std::endl;
//    test = lol.slice(0, 2, 3, 4);
//    std::cout << nnt.predict(test) << std::endl;

    Table a = getFromCsv("/home/arseniyx92/Desktop/c++/neural_net_lib/training_mush.csv");
    Table X = a.slice(0, 10, 5200, 6000);
    Table Y = a.slice(22, 23, 5200, 6000);
    Net nt(10, 2, 2, {5, 2});
    nt.fit(X, Y, 2000);
    nt.Zip("netZipped.txt");

    Table Xtest = a.slice(0, 10, 0, a.get_rows());
    Table Ytest = a.slice(a.get_cols()-1, a.get_cols(), 0, a.get_rows());
    Net nnt;
    nnt.UnZip("netZipped.txt");
    std::cout << nnt.accuracy(Xtest, Ytest) << std::endl;
    return 0;
}
