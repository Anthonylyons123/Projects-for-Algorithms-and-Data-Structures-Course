#include <iostream>

#include "time_series_class.hpp"

#ifndef linked_list
#define linked_list

class Linkedlist {
    private:
    Linkedlist();
    ~Linkedlist();
    void load(std::string country, std::string filename);
    void list();
    void add(std::string series_code, int year, double data);
    void update(std::string series_code, int year, double data);
    void print(std::string series_code);
    void DELETE(std::string series_code);
    void biggest();

    //Helper Functions
    void deletelinkedlist();
    double series_code_mean(std::string Series_Code);


    Time_Series *m_head;
    std::string m_country_name;

    friend class Tree;
};

#endif