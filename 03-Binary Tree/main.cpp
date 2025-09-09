#include <iostream> 
#include <sstream>
#include <fstream>

#include "time_series_class.hpp"
#include "linked_list.hpp"
#include "tree.hpp"

int main();

int main(){

    Tree temp;
    std::string command;

    while (true){
        std::cin >> command;
        if (command == "LOAD"){
            std::cin >> command; 
            temp.Tree_load(command);
            std::cout << "success" <<std::endl;
        } else if (command == "LIST"){
            std::cin >> command;
            temp.Tree_list(command);
        } else if (command == "RANGE"){
            std::cin >> command;
            temp.Tree_range(command);
        } else if (command == "BUILD"){
            std::cin >> command;
            temp.Tree_build(command);
            std::cout << "success" << "\n";
        } else if (command == "FIND"){
            double mean;
            std::cin >> mean;
            std::cin >> command;
            temp.Tree_find(mean, command);
        } else if (command == "DELETE"){
            std::cin >> command;
            temp.Tree_delete(command);
        } else if (command == "LIMITS"){
            std::cin >> command;
            temp.Tree_limits(command);
        } else if (command == "EXIT"){
            return 0;
        }
    }


    // Linkedlist temp;
    // std::string command;
    // while (true){
    //     std::cin >> command;
    //     if (command == "LOAD"){ //Calls LOAD function
    //         std::cin >> command; 
    //         temp.load(command);
    //         std::cout << "success" <<std::endl;
    //     } else if (command == "LIST"){
    //         temp.list();
    //     } else if (command == "ADD"){ //Calls Add function and gets year and data inputs from user
    //         int year{0};
    //         double data{0};
    //         std::cin >> command;
    //         std::cin >> year;
    //         std::cin >> data;
    //         temp.add(command, year, data);
    //     } else if (command == "UPDATE"){ //Calls Update function and gets year and data inputs from user
    //         int year{0};
    //         double data{0};
    //         std::cin >> command;
    //         std::cin >> year;
    //         std::cin >> data;
    //         temp.update(command, year, data);
    //     } else if (command == "PRINT"){
    //         std::cin >> command;
    //         temp.print(command);
    //     } else if (command == "DELETE"){ //Calls Mean function
    //         std::cin >> command;
    //         temp.DELETE(command);
    //     } else if (command == "BIGGEST"){ //Calls monotonic fucntion
    //         temp.biggest();
    //     } else if (command == "EXIT"){ //Terminates the program if exit is called
    //         return 0;
    //     }
    //     }
}