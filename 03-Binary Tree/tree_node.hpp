#include <iostream>

#ifndef tree_node
#define tree_node

class Tree_Node{

    private:
    Tree_Node(){
        m_left = nullptr;
        m_right = nullptr;
    }
    //~Tree_Node();
    Tree_Node *m_left;
    Tree_Node *m_right;
    double m_max_range{0};
    double m_min_range{0};
    int m_min_mean_loc{0};
    int m_max_mean_loc{0};

    friend class Tree;

};

#endif