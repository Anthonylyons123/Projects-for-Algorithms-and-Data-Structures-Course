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
        } else if (command == "INITIALIZE"){
            temp.Tree_initialize(); 
        } else if (command == "UPDATE_EDGES"){
            std::string series;
            double rel;
            std::cin >> series;
            std::cin >> rel;
            std::cin >> command;
            temp.Tree_updateedges(series, rel, command);
        } else if (command == "ADJACENT"){
            std::cin >> command;
            temp.Tree_adjacent(command);
        } else if (command == "PATH"){
            std::string C1;
            std::cin >> C1;
            std::cin >> command;
            temp.Tree_path(C1, command);
        } else if (command == "RELATIONSHIPS"){
            std::string C1;
            std::cin >> C1;
            std::cin >> command;
            temp.Tree_relationships(C1, command);
        } else if (command == "EXIT"){
            return 0;
        }
    }

}