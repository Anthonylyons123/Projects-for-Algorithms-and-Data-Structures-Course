#include <iostream>

#include "linked_list.hpp"

#ifndef tree
#define tree

class Tree {
    public:
    Tree();
    ~Tree();
    void Tree_load(std::string filename);
    void Tree_list(std::string Country_Name);
    void Tree_range(std::string Series_Code);
    void Tree_build(std::string Series_Code);
    void Tree_find(double mean, std::string opperation);
    void Tree_delete(std::string Country_Name);
    void Tree_limits(std::string condition);

    Time_Series *m_arr_point[512];
    std::string *m_country_loc; //Stores where the any given country is in the array

};

#endif