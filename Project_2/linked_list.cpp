#include <iostream>
#include <sstream>
#include <fstream>

#include "linked_list.h"

Linkedlist::Linkedlist(){ //Constructor
    m_head = nullptr; 
}

Linkedlist::~Linkedlist(){ //Destructor

}

void Linkedlist::load(std::string country){
    std::string line; 
    std::string next_line;

    Time_Series* node = new Time_Series;
    m_head = node;

    std::ifstream info_file ("lab2_multidata.csv");
    std::getline(info_file, line); //loads the first line of the lab2_multidata file into line
/*
    CITATION:
    I was asking chatgbt how to check if a string is empty for a condition

    Promt 1:
    how do you compare a string with an empty string for the condition 
    to break out of a while loop. This did not give the result I was looking for so
    asked a follow up question
    Promt 2:
    I mean like the condition of the while loop would be while (string != "empty string")

    Chatgbt then told me to use while (string != "")
*/        
    while (line != ""){ 
        std::stringstream tempstring1(line); //converts line into a string stream called tempstring1
        std::getline (tempstring1, node->m_c_name,','); //Assigns node m_c_name to whatever country is in the tempstring2
        if ((node->m_c_name)== country){ //checks if the country is the country we are looking for
            
            node->LOAD(line); //Calls Time_Series LOAD function can populates the node with infomation
            node->m_next = new Time_Series; //Creating a new node to be the "next" node in the list
        
            std::getline(info_file, next_line); //Geting the next line of the lab2_multidata file
            std::stringstream tempstring2(next_line); //converts next_line into a string stream called tempstring2
            std::string next_line_country_name;
            std::getline(tempstring2, next_line_country_name, ','); //gets the country name of the next line

            if (next_line_country_name == country){ //checks if the next line is the country we want
                node = node->m_next; //If it is the country we want nodes m_next points to the new node
            } else {
                node->m_next = nullptr; //If it isnt the country we want we are at the end of the list and therfore point the node to nullptr
                return;
            }
            line = next_line; //assigning line to the next line so we don't skip any lines in the lab2_multidata file
        
        } else {
            std::getline(info_file, line);
        }
    }
    m_head = nullptr;
}

void Linkedlist::list(){    
    
    Time_Series *current;
    current = m_head;
    if (current == nullptr){
        return;
    }
    
    std::cout << current->m_c_name << " " << current->m_c_code << " " << current->m_s_name << " ";
    while (current->m_next != nullptr){
        current = current->m_next;
        std::cout << current->m_s_name << " ";
    }
}

void Linkedlist::add (std::string series_code, double data, int year){
    Time_Series *looper;
    looper = m_head;
    while (looper != nullptr){
        if (looper->m_s_code == series_code){
            looper->ADD(data, year);
            return;
        }
        looper = looper->m_next;
    }
}

void Linkedlist::update(std::string series_code, double data, int year){
    Time_Series *looper;
    looper = m_head;
    while (looper != nullptr){
        if (looper->m_s_code == series_code){
            looper->UPDATE(data, year);
            return;
        }
        looper = looper->m_next;
    }
}

void Linkedlist::print(std::string series_code){
    Time_Series *looper;
    looper = m_head;
    while (looper != nullptr){
        if (looper->m_s_code == series_code){
            looper->PRINT();
            return;
        }
        looper = looper->m_next;
    }
}