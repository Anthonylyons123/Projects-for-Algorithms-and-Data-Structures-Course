#include <iostream>
#include <map>
#include <list>
#include <vector>

#include "linked_list.hpp"
#include "tree_node.hpp"

#ifndef tree
#define tree

struct Relationship {
    std::string seriescode;
    double relation;
    std::string threshold;
};

class Tree {
    public:
    Tree();
    ~Tree();
    void Tree_load(std::string filename);
    void Tree_build(std::string Series_Code);
    void Tree_find(double mean, std::string opperation);
    void Tree_initialize();
    void Tree_updateedges(std::string seriescode, double threshold, std::string relation);
    void Tree_adjacent(std::string countrycode);
    void Tree_path(std::string cCode1, std::string cCode2);
    void Tree_relationships(std::string cCode1, std::string cCode2);

    private:
    //Member variables

    std::map <std::string, std::map <std::string, std::vector<Relationship>>> m_graph;
    std::vector<std::string> m_relationvec;
    int m_relationvec_size {0};

    Linkedlist *m_list[512];
    double *m_mean_values{nullptr}; 
    int m_items_stored{0};
    std::string m_series_code; //Used for the delete recursive function only, get assigned the string inside build
    Tree_Node *root{nullptr};

    //Helper Functions
    void Tree_populate_mean_array(std::string Series_Code);
    void Tree_sortingalgorithm(double *array_to_sort);
    void Tree_find_mean_loaction(Tree_Node *Node, double min_mean, double max_mean);
    Tree_Node* Tree_build_recursive(std::string Series_Code, Tree_Node *node, double min_mean, double max_mean);
    void Tree_find_recursive(Tree_Node *node, double mean, std::string operation);
    void Tree_post_order_delete(Tree_Node *node);
    
};

#endif