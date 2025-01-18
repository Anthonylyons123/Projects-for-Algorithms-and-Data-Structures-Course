#include <iostream> 
#include <sstream>
#include <fstream>
#include "time_series_class.h"

int main();

int main(){
    Time_Series temp;
    temp.LOAD("lab1_canada.csv");
    temp.PRINT();
    std::cout << std::endl;
    std::cout << std::endl;
    

    temp.ADD(20, 1959);
    temp.ADD(1234, 1938);
    temp.PRINT();
    std::cout << "\n";
    std::cout<< temp.m_arr_size;

    // std::cout << std::endl;
    // temp.ADD(123, 1983);
    // temp.PRINT();
    //   std::cout << std::endl;

    // std::cout << std::endl;
    // temp.ADD(999, 4000);
    // temp.PRINT();
    // std::cout << std::endl;

    //std::cout << temp.mean();

    return 0;
}