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
        if (command == "LOAD"){ //Calls LOAD function
            std::cin >> command; 
            temp.LOAD(command);
            std::cout << "success" <<std::endl;
        } else if (command == "PRINT"){ //Calls Print Function
            temp.PRINT();
        } else if (command == "ADD"){ //Calls Add function and gets year and data inputs from user
            int year{0};
            double data{0};
            std::cin >> year;
            std::cin >> data;
            temp.ADD(data, year);
        } else if (command == "UPDATE"){ //Calls Update function and gets year and data inputs from user
            int year{0};
            double data{0};
            std::cin >> year;
            std::cin >> data;
            temp.UPDATE(data, year);
        } else if (command == "MEAN"){ //Calls Mean function
            temp.mean();
        } else if (command == "MONOTONIC"){ //Calls monotonic fucntion
            temp.is_monotonic();
        } else if (command == "FIT"){ //Calls fit funtion
            double m{0};
            double b{0};
            temp.best_fit(m, b);
        } else if (command == "EXIT"){ //Terminates the program if exit is called
            return 0;
        }
    }
}