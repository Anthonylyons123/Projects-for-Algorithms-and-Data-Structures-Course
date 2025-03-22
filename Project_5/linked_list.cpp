#include <iostream>
#include <sstream>
#include <fstream>

#include "linked_list.hpp"

Linkedlist::Linkedlist(){ //Constructor
    m_head = nullptr; 
}

Linkedlist::~Linkedlist(){ //Destructor
    deletelinkedlist();
}

void Linkedlist::load(std::string country, std::string filename){
    m_country_name = country;
    std::string line; 
    std::string next_line;

    deletelinkedlist(); //Delete the existing linked list
    
    Time_Series* node = new Time_Series; //Creating the first node of a new linked list
    m_head = node;

    std::ifstream info_file (filename);
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
                delete node->m_next;
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
    if (current == nullptr){ //Check if list is empty;
        return;
    }
    
    std::cout << current->m_c_name << " " << current->m_c_code << " " << current->m_s_name << " "; //Print the country name, code and the first series name
    while (current->m_next != nullptr){ //Loop through the rest of the linked list and print the rest of the series names
        current = current->m_next; 
        std::cout << current->m_s_name << " ";
    }
    std::cout << "\n";
}

void Linkedlist::add (std::string series_code, int year, double data){
    Time_Series *looper;
    looper = m_head;
    while (looper != nullptr){ //Loop through linked list
        if (looper->m_s_code == series_code){ //Check if the series code at the node we are on is the same as the series code that we want to add data to
            looper->ADD(data, year); //If we find the series code that was input in the function call the ADD function from Time_Series class
            return;
        }
        looper = looper->m_next;
    }
    std::cout << "failure" << std::endl; //Print failure if list is empty or if the inputed series code isnt in the linked list
}

void Linkedlist::update (std::string series_code, int year, double data){
    Time_Series *looper;
    looper = m_head;
    while (looper != nullptr){ //Loop through linked list
        if (looper->m_s_code == series_code){ //Check if the series code at the node we are on is the same as the series code that we want to update data
            looper->UPDATE(data, year); //If we find the series code that was input in the function call the UPDATE function from Time_Series class
            return;
        }
        looper = looper->m_next;
    }
    std::cout << "failure" << std::endl; //Print failure if list is empty or if the inputed series code isnt in the linked list
}

void Linkedlist::print(std::string series_code){
    Time_Series *looper;
    looper = m_head;
    while (looper != nullptr){ //Loop through linked list
        if (looper->m_s_code == series_code){ //Check if the series code at the node we are on is the same as the series code that we want to Print
            looper->PRINT(); //If we find the series code that was input in the function call the PRINT function from Time_Series class
            return;
        }
        looper = looper->m_next;
    }
    std::cout << "failure" << std::endl; //Print failure if list is empty or if the inputed series code isnt in the linked list
}

void Linkedlist::DELETE(std::string series_code){
    Time_Series *before = m_head;
    Time_Series *current = m_head;
    Time_Series *after = m_head;
    
    if (current == nullptr){ //Nothing in the list
        std::cout << "failure" <<std::endl;
        return;
    } else {
        if (current->m_s_code == series_code){ // Case 1: Node we want to delete is the first node in the list
            if (current->m_next == nullptr){ //Case 1a: Only one node in the linked list
                delete current;
                m_head = nullptr;
                std::cout << "success" <<std::endl;
                return;
            } else { //Case 1b: More than one node in the list
                after = after->m_next; //Go to the next node in the list
                delete current; // Delete the node we want
                current = nullptr; 
                m_head = after; //Assign the head to the new first node in the list
                std::cout << "success" <<std::endl;
                return;
            }

        } else { // Case 2: Node we want isnt the first node in the list
            current = current->m_next;
            after = after->m_next;
            while (current != nullptr){  
                if (current->m_s_code == series_code){
                    after = after->m_next; //Go to the next node in the list
                    delete current; // Delete the node we want
                    current = nullptr; 
                    before->m_next = after; //Point the node before the node we want to the node after the one we want
                    std::cout << "success" <<std::endl;
                    return;
                }
                //Going to the next node in the list
                before = before->m_next;
                current = current->m_next;
                after = after->m_next;
            }
        }
    }
    std::cout << "failure" <<std::endl;
}

void Linkedlist::biggest(){
    std::string series_code;
    double biggest_mean{0};
    double new_mean{0};

    Time_Series *looper;
    looper = m_head;

    if (looper == nullptr){ //Empty list
        std::cout << "failure" << std::endl;
        return;
    } else {
        while (looper != nullptr){ //Loop through list
            for (int n{0}; n < looper->m_count; ++n){ //Adding up all of the values stored in my data array
                new_mean += looper->arr_data[n];
            }
            new_mean = (new_mean/looper->m_count); //Divide by the total number of elements to get the mean

            if (new_mean > biggest_mean){ //Checking if "new mean" is greater than "biggest mean", if so assign biggest mean the value of new mean
                biggest_mean = new_mean;  // and update the corresponding series code that corresponds to the biggest mean
                series_code = looper->m_s_code;
            } 
            looper = looper->m_next;
            new_mean = 0; //Reset new mean when we go to the next node in the list
        }        
        if (biggest_mean != 0){ //Print the series code with the biggest mean or failure if there was no mean to calculate
            std::cout << series_code << std::endl;
        } else {
            std::cout << "failure" <<std::endl;
        }
    }

}

//Helper functions 
void Linkedlist::deletelinkedlist(){
    Time_Series *current = m_head;
    Time_Series *temp = m_head;
    while (current != nullptr){ //Loop through linked list and delete the nodes
        temp = current->m_next;
        delete current;
        current = temp;
    }
    m_head = nullptr; //update head to point to nullptr
}

double Linkedlist::series_code_mean(std::string Series_Code){
    Time_Series *temp = m_head;
    double mean{0};

    while (temp != nullptr){
        if ((temp->m_s_code == Series_Code) && (temp->m_count > 0)){ //Find the series code that we are looking for and make sure that we have valid data for this timeseries
            for (int n{0}; n < temp->m_count; ++n){ //Adding up all of the values stored in the data array
                mean += temp->arr_data[n];
            }
            return mean /= temp->m_count; //Return the mean if we have valid data 
        }
        temp = temp->m_next; //Loop through countries linked list
    }
    return -1; //Return -1 if no valid data

}