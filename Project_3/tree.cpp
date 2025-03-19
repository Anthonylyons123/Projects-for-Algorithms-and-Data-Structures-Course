#include <iostream>
#include <sstream>
#include <fstream>

#include "tree.hpp"

Tree::Tree(){

}

Tree::~Tree(){
// Delete m_list array  
for (int i{0}; i < m_items_stored; i++) delete m_list[i];
if (m_mean_values != nullptr) delete[] m_mean_values;
Tree_post_order_delete(root);
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
            m_list[iterator] = new Linkedlist; // Creating new memory inside the array
            m_list[iterator]->load(firstword, filename); //Filling up the array
            m_list[iterator]->m_country_name = firstword;
            iterator++; 
            m_items_stored++; //Keep count of how many linked lists are stored in the array
            temp_country = firstword; //temp_country is assigned the string in firstword so that we don't duplicate countries linked lists
        }
    }
}

void Tree::Tree_list(std::string Country_Name){
    for (int i = 0; i < m_items_stored; i++) {
        if (m_list[i]->m_country_name == Country_Name) {
            m_list[i]->list();
        }
    }
}
void Tree::Tree_range(std::string Series_Code){
    double min_mean{0};
    double max_mean{0};
    double temp_mean{0};

    for (int i{0}; i < m_items_stored; i++){ //Loop through each countries linked list
        temp_mean = m_list[i]->series_code_mean(Series_Code); //Calculate the mean of that series code for each countries linked list
        if (i == 0){
            min_mean = max_mean = temp_mean;
        } else {
            if ((temp_mean > max_mean) && (temp_mean >= 0)){
                max_mean = temp_mean; //update max mean
            } else if ((temp_mean < min_mean) && (temp_mean >= 0)){
                min_mean = temp_mean; //update min mean
            }
        }
    }
    std::cout << min_mean << " " << max_mean <<std::endl;
}

void Tree::Tree_build(std::string Series_Code){
    Tree_post_order_delete(root); //Delete previos tree
    m_items_del = 0; //Reset this value every time a tree is being built from a new series code

    m_series_code = Series_Code; //Will need this for the deleting the tree

    for (int i{0}; i < m_items_stored; i++){ //Moving all pointers in m_list to m_list_del
        m_list_del[i] = m_list[i];
    }

    Tree_populate_mean_array(Series_Code); // Sorting the mean values
    root = new Tree_Node;
    Tree_build_recursive(Series_Code, root, m_mean_values[0], m_mean_values[m_items_stored-m_items_del-1]);
    //print(root);  

}
void Tree::Tree_find(double mean, std::string opperation){

    Tree_find_recursive(root, mean, opperation); //Pass the root, mean, and operation into the recursive function
    std::cout << "\n";

}

void Tree::Tree_delete(std::string Country_Name){
    Tree_delete_recursive(m_series_code, Country_Name); //Pass the root, and country name into the recursive function
}
void Tree::Tree_limits(std::string condition){
    Tree_limits_recursive(root, condition); //Pass the root, and condition into the recursive function
    std::cout << "\n";
}


//Helper Functions
void Tree::Tree_populate_mean_array(std::string Series_Code){
    
    if (m_mean_values != nullptr){
        delete[] m_mean_values; //Delete m_values_array when making a new tree
    }
    m_mean_values = new double[m_items_stored];

    for (int i{0}; i < m_items_stored-m_items_del; i++){
        m_mean_values[i] = m_list_del[i]->series_code_mean(Series_Code); //Calculates the mean for each country
        if (m_mean_values[i] == -1){ //Dealing with invaid data
            m_mean_values[i] = 0;
        }
    }
    Tree_sortingalgorithm(m_mean_values);
}

void Tree::Tree_sortingalgorithm(double *array_to_sort){

    for (int i = 0; i < m_items_stored-m_items_del-1; i++){ //items -1 since we only want to go up to the second last array element
        bool check = false;
        for (int j = 0; j < m_items_stored-m_items_del-1-i; j++){ //subtract i since the end of the array is sorted
            if (array_to_sort[j] > array_to_sort[j + 1]){ //Moving the largers means to the end of the array
                // Swap values in array_to_sort
                double temp = array_to_sort[j];
                array_to_sort[j] = array_to_sort[j + 1];
                array_to_sort[j + 1] = temp;

                // Swap the corresponding countries linked list
                Linkedlist *tempList = m_list_del[j];
                m_list_del[j] = m_list_del[j + 1];
                m_list_del[j + 1] = tempList;

                check = true;
            }
        }
        
        if (check == false){ //Nothing to swap means the array is sorted
            break;
        }

    }

}

void Tree::Tree_find_mean_loaction(Tree_Node *Node, double min_mean, double max_mean){
    
    //Finding the array index for the minimum mean and maximum mean value
    for (int i{0}; i < m_items_stored-m_items_del; i++){

        if (m_mean_values[i] < min_mean){
            Node->m_min_mean_loc = i+1;
        }
        if (m_mean_values[i] <= max_mean){
            Node->m_max_mean_loc = i;
        }
    }
}

Tree_Node* Tree::Tree_build_recursive(std::string Series_Code, Tree_Node *node, double min_mean, double max_mean){

    Tree_find_mean_loaction(node, min_mean, max_mean); //Finding array index for min and max mean value
    node->m_min_range = min_mean; 
    node->m_max_range = max_mean;

    if ((m_mean_values[node->m_max_mean_loc]-m_mean_values[node->m_min_mean_loc] <= 1E-3) || ((node->m_max_mean_loc - node->m_min_mean_loc)+1 <= 1)){ // If the difference between min and max mean in less than 0.001 they can go in the same leaf node
        return node;
    }

    double mid = (min_mean + max_mean) / 2; // Calculate the mid point

    node->m_left = Tree_build_recursive(Series_Code, new Tree_Node, min_mean, mid); //Recurse left
    node->m_right = Tree_build_recursive(Series_Code, new Tree_Node, mid, max_mean); //Recurse right

    return node;
}


void Tree::print(Tree_Node *node) {
    if (node == nullptr) return;
    print(node->m_left);
    for (int i = node->m_min_mean_loc; i <= node->m_max_mean_loc; i++) {
        std::cout << m_list_del[i]->m_country_name << ", ";
    }
    print(node->m_right);
}

void Tree::Tree_find_recursive(Tree_Node *node, double mean, std::string operation){
    
    if (root == nullptr){ //empty tree
        std::cout << "failure"; 
        return;
    }

    if (operation == "equal"){
        if (node == nullptr){ //Return if nullptr
            return; 
        } 
        
        if (node->m_right == nullptr && node->m_left == nullptr){ //We only print if we get to a leaf node
            if (std::abs(m_mean_values[node->m_min_mean_loc]-mean) <= 1E-3){ //Check if mean = mean values in the array within the 1E-3 tolorance
                for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                    std::cout << m_list_del[i]->m_country_name << " ";
                }
                return;
            }
        } 
        Tree_find_recursive (node->m_left, mean, operation);
        Tree_find_recursive (node->m_right, mean, operation);
        
        return;

    } else if (operation == "less"){
        if (node == nullptr){
            return;
        } 
        
        if (node->m_right == nullptr && node->m_left == nullptr){ // If both left and right are null, theres no way to check the mean with the values stored at that node so you have to manually do it
            if (m_mean_values[node->m_min_mean_loc] < mean){ //If the mean values in the array < inputed mean value print the countries
                for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                    std::cout << m_list_del[i]->m_country_name << " ";
                }
                return;
            }
        } 
        
        if (mean > node->m_max_range){ //If mean > max mean value all the values are less than
            for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                std::cout << m_list_del[i]->m_country_name << " ";
            }
            return;
        } 
        
        if (mean < node->m_min_range){ //If mean < min mean value, there is no data less than mean
            return;
        }
        Tree_find_recursive (node->m_left, mean, operation);
        Tree_find_recursive (node->m_right, mean, operation);
        return;

    } else if (operation == "greater"){
        if (node == nullptr){
            return;
        }
        
        if (node->m_right == nullptr && node->m_left == nullptr){ // If both left and right are null, theres no way to check the mean with the values stored at that node so you have to manually do it
            if (m_mean_values[node->m_min_mean_loc] > mean){ //If the mean values in the array > inputed mean value print the countries
                for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                    std::cout << m_list_del[i]->m_country_name << " ";
                }
            }
            return;
        } 
        
        if (mean < node->m_min_range){ //If mean < min mean value then all mean values are greater than mean
            for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                std::cout << m_list_del[i]->m_country_name << " ";
            }
            return;
        }
        
        if (mean > node->m_max_range){ //If m > max mean value, then there are no mean values < mean so return
            return;
        }

        Tree_find_recursive (node->m_left, mean, operation);
        Tree_find_recursive (node->m_right, mean, operation);
        return; 
    }

}

void Tree::Tree_delete_recursive(std::string series_code, std::string country){
    bool check = false;
    if (root == nullptr){ //empty tree
        std::cout << "failure" << "\n";
        return;
    }
    for (int i{0}; i < m_items_stored-m_items_del; i++){
        if (m_list_del[i]->m_country_name == country){ //Checking if the country being removed is in that node
            for (int j{i}; j < m_items_stored-m_items_del-1; j++){ //If the country is in that node shift all of the entries in the mean and m_list_del array left 1
                m_list_del[j] = m_list_del[j+1];
                m_mean_values[j] = m_mean_values[j+1];
            }
            m_items_del++; //Count how many countries have been deleted from the tree
            check = true;
        }
    }
    if (check == true){    
        std::cout << "success" << "\n";
    } else {
        std::cout << "failure" << "\n";
    }

    Tree_post_order_delete(root); //delete the tree
    Tree_build_recursive(series_code, root = new Tree_Node, m_mean_values[0], m_mean_values[m_items_stored-m_items_del-1]); //Rebuild the tree without the country we deleted
}

void Tree::Tree_limits_recursive(Tree_Node *node, std::string condition){

    if (root == nullptr){ //empty tree
        std::cout << "failure";
        return;
    } 

    if (condition == "lowest"){
        if (node == nullptr){
            return;
        } 

        if (node->m_left == nullptr){ //Print countries in left most node
            for(int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                std::cout << m_list_del[i]->m_country_name << " ";
            }
        }

        Tree_limits_recursive (node->m_left, condition); //Recurse left

    } else if (condition == "highest"){
        if (node == nullptr){
            return;
        } 
        
        if (node->m_right == nullptr){ //Print countries in right most node
            for(int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                std::cout << m_list_del[i]->m_country_name << " ";
            }
        }
  
        Tree_limits_recursive (node->m_right, condition); //Recurse right
    }
}

void Tree::Tree_post_order_delete(Tree_Node *node){
    
    // Delete the tree using a post oder traversial
    if (node == nullptr){
        return;
    } else {
        Tree_post_order_delete(node->m_left);
        Tree_post_order_delete(node->m_right);
        delete node;
    }

}