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
            temp.is_monotonic();
        } else if (command == "FIT"){
            double m{0};
            double b{0};
            temp.best_fit(m, b);
        } else if (command == "EXIT"){
            return 0;
        }
    }
}