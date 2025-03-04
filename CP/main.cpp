#include <iostream>
#include "include/DAG.h"

int main() {
    DAG dag("dag.ini");
    if (!dag.is_valid()) {
        std::cout << "DAG is invalid!" << std::endl;
        return 1;
    }
    //std::cout << "main2\n";
    dag.execute(4);
    //std::cout << "main3\n";
    return 0;
}