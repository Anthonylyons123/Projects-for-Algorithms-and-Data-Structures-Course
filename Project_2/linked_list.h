#include <iostream>

#include "time_series_class.h"

#ifndef linked_list
#define linked_list

class Linkedlist {
    public:
    Linkedlist();
    ~Linkedlist();
    void load(std::string country);
    void list();
    void add(std::string series_code, double data, int year);
    void update(std::string series_code, double data, int year);
    void print(std::string series_code);
    void DELETE(std::string series_code);
    void biggest();

    private:
    Time_Series *m_head;
};

#endif