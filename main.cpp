#include "utils/utils.h"
#include "table/table.h"
#include "csv_serializer/csv_serializer.h"
#include "neural_net/neural_net.h"

int main() {
//    Table a = getFromCsv("/home/arseniyx92/Desktop/c++/neural_net_lib/training_mush.csv");
//    Table b = a.slice(3, 5, 2, 5);

    Table lol = getFromCsv("/home/arseniyx92/Desktop/c++/neural_net_lib/xor.csv");
    Table X = lol.slice(0, 2, 0, 4);
    Table Y = lol.slice(2, 3, 0, 4);
    Net nt(2, 2, 1, {2});
    nt.fit(X, Y, 2000);

    Table test = lol.slice(0, 2, 0, 1);
    std::cout << nt.predict(test) << std::endl;
    test = lol.slice(0, 2, 1, 2);
    std::cout << nt.predict(test) << std::endl;
    test = lol.slice(0, 2, 2, 3);
    std::cout << nt.predict(test) << std::endl;
    test = lol.slice(0, 2, 3, 4);
    std::cout << nt.predict(test) << std::endl;

//    Table a = getFromCsv("/home/arseniyx92/Desktop/c++/neural_net_lib/training_mush.csv");
//    Table X = a.slice(0, 22, 0, 6000);
//    Table Y = a.slice(22, 23, 0, 6000);
//    Net nt(22, 2, 2, {10, 10});
//    nt.fit(X, Y);
    return 0;
}
