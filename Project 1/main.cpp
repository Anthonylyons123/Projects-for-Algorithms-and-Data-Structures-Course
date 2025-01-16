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
    std::cout << temp.mean();
    // Time_Series test;

    // test.s_name = string.substr(11, 39); //How can you make this general
    // test.s_code = string.substr(51,14);  //How can you loop through the string
    //                                      //to find where the start of s_name is?

    // std::cout<< test.s_name <<std::endl;
    // std::cout<< test.s_code <<std::endl;

    return 0;
}