#include <iostream> 
#include <sstream>
#include <fstream>
#include "time_series_class.h"

int main();

int main(){
    Time_Series temp;
    std::string command;
    while (true){
        std::cin >> command;
        if (command == "LOAD"){
            std::cin >> command; 
            temp.LOAD(command);
            std::cout << "success" <<std::endl;
        } else if (command == "PRINT"){
            temp.PRINT();
        } else if (command == "ADD"){
            int year{0};
            double data{0};
            std::cin >> year;
            std::cin >> data;
            temp.ADD(data, year);
        } else if (command == "UPDATE"){
            int year{0};
            double data{0};
            std::cin >> year;
            std::cin >> data;
            temp.UPDATE(data, year);
        } else if (command == "MEAN"){
            temp.mean();
        } else if (command == "MONOTONIC"){
            if (temp.is_monotonic()){
                std::cout << "series is monotonic" <<std::endl;
            } else {
                std::cout << "series is not monotonic" <<std::endl;
            }
        } else if (command == "EXIT"){
            return 0;
        }
    }

    // temp.LOAD("lab1_canada.csv");
    // temp.PRINT();

    // std::cout << std::endl;
    // std::cout << std::endl;
    // std::cout << std::endl;

    // //temp.UPDATE(-1,1970);
    // //temp.PRINT();

    // std::cout << std::endl;

    // double a{10};
    // double b{9};
    // std::cout << temp.is_monotonic() << std::endl;
    // temp.best_fit(a, b);
    // std::cout<< a <<", "<<b;

    //  temp.ADD(20, 1961);
    //  temp.ADD(1234, 1938);
    //  temp.PRINT();
    //  std::cout << "\n";
 
    // // std::cout << std::endl;
    // // temp.ADD(123, 1983);
    // // temp.PRINT();
    // //   std::cout << std::endl;

    // // std::cout << std::endl;
    // // temp.ADD(999, 4000);
    // // temp.PRINT();
    // // std::cout << std::endl;

    // //std::cout << temp.mean();

    return 0;
}