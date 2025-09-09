#include <iostream>

#include "linked_list.hpp"
#include "tree_node.hpp"

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
    void Tree_lookup(std::string countrycode);
    void Tree_remove(std::string countrycode);
    void Tree_insert(std::string countrycode, std::string filename);

    
    private:
    //Member variables
    Linkedlist *m_list[512] {nullptr};
    Linkedlist *m_list_del[512] {nullptr};
    int m_items_stored{0};
    int m_items_del{0};
    bool m_stored_or_not[512] {false}; //An array full of bool values where 1 is theres a linked list currently there, and 0 means theres no linked list there.
    std::string m_temp_arr;

    double *m_mean_values {nullptr}; 
    int *m_sorted_mean_index {nullptr};
    
    std::string m_series_code; //Used for the delete recursive function only, get assigned the string inside build
    Tree_Node *root{nullptr};

    //Helper Functions
    void Tree_populate_mean_array(std::string Series_Code);
    void Tree_sortingalgorithm();
    void Tree_find_mean_loaction(Tree_Node *Node, double min_mean, double max_mean);
    Tree_Node* Tree_build_recursive(std::string Series_Code, Tree_Node *node, double min_mean, double max_mean);
    void print(Tree_Node *node);
    void Tree_find_recursive(Tree_Node *node, double mean, std::string operation);
    void Tree_delete_recursive(std::string series_code, std::string country, bool func_call);
    void Tree_limits_recursive(Tree_Node *node, std::string condition);
    void Tree_post_order_delete(Tree_Node *node);

    int Tree_prime_Hash();
    int Tree_sec_Hash(int W);

    int Tree_search_table(std::string countrycode, bool check);
    bool Tree_is_CCode_stored(std::string countrycode);

};

#endif