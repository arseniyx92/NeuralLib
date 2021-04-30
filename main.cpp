#include "utils/utils.h"
#include "table/table.h"
#include "csv_serializer/csv_serializer.h"

int main() {
    Table a = getFromCsv("/home/arseniyx92/Desktop/c++/neural_net_lib/training_mush.csv");
    Table b = a.slice(3, 5, 2, 5);
    return 0;
}
