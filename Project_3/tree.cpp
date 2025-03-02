#include <iostream>
#include <sstream>
#include <fstream>

#include "tree.hpp"

Tree::Tree(){
    m_country_loc = new std::string [512];
}

Tree::~Tree(){
// Delete binary tree   
}

void Tree::Tree_load(std::string filename){
    std::string line;
    std::string temp_country {" "};
    int iterator{0};

    std::ifstream info_file (filename); 
    while (std::getline(info_file, line)){ //read each line of csv file
        std::stringstream country_name(line);
        std::string firstword;
        std::getline(country_name, firstword, ',');

        if (firstword != temp_country) { //New country
            Linkedlist temp;
            temp.load(firstword); //Calls linked list load function to create a linked list for that country
            m_arr_point[iterator] = temp.m_head; //storing the head of linked list
            m_country_loc[iterator] = firstword; //array at iterator is assigned the head of that countries linked list
            iterator++; 
            temp_country = firstword; //temp_country is assigned the string in firstword so that we don't duplicate countries linked lists;
        }
    }
}
void Tree::Tree_list(std::string Country_Name){

}
void Tree::Tree_range(std::string Series_Code){

}
void Tree::Tree_build(std::string Series_Code){

}
void Tree::Tree_find(double mean, std::string opperation){

}
void Tree::Tree_delete(std::string Country_Name){

}
void Tree::Tree_limits(std::string condition){

}
