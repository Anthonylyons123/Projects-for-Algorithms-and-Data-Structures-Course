#include <iostream>

#include "time_series_class.hpp"

#ifndef linked_list
#define linked_list

class Linkedlist {
    private:
    Linkedlist();
    ~Linkedlist();
    void load(std::string country);
    void list();
    void add(std::string series_code, int year, double data);
    void update(std::string series_code, int year, double data);
    void print(std::string series_code);
    void DELETE(std::string series_code);
    void biggest();
    void deletelinkedlist();

    Time_Series *m_head;

    friend class Tree;
};

#endif