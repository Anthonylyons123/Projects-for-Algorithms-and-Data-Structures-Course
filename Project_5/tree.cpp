#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include <set>

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
        std::string secondword;
        std::getline(country_name, secondword, ',');

        if (firstword != temp_country) { //New country
            m_list[iterator] = new Linkedlist; // Creating new memory inside the array
            m_list[iterator]->load(firstword, filename); //Filling up the array
            m_list[iterator]->m_country_name = firstword; 
            m_list[iterator]->m_country_code = secondword;

            iterator++; 
            m_items_stored++; //Keep count of how many linked lists are stored in the array
            temp_country = firstword; //temp_country is assigned the string in firstword so that we don't duplicate countries linked lists
        }
    }
}

void Tree::Tree_build(std::string Series_Code){
    Tree_post_order_delete(root); //Delete previos tree

    m_series_code = Series_Code; //Will need this for the deleting the tree

    Tree_populate_mean_array(Series_Code); // Sorting the mean values
    root = new Tree_Node;
    Tree_build_recursive(Series_Code, root, m_mean_values[0], m_mean_values[m_items_stored-1]);
    //print(root);  

}
void Tree::Tree_find(double mean, std::string opperation){

    Tree_find_recursive(root, mean, opperation); //Pass the root, mean, and operation into the recursive function
    //std::cout << "\n";
}


/*
---------------------------------------------Project 5 Functions------------------------------------------------------------
*/


void Tree::Tree_initialize(){

    for (int i{0}; i < m_items_stored; i++){
        m_graph[m_list[i]->m_country_name] = {}; //Making a disconnected graph
    }

    std::cout << "success" << std::endl;

}

void Tree::Tree_updateedges(std::string seriescode, double threshold, std::string relation){
    bool new_rel_or_not{false};
    m_relationvec.clear();
    m_relationvec_size = 0;

    Tree_build(seriescode);
    Tree_find(threshold, relation);


    Relationship temp = {seriescode, threshold, relation};

    // Checking to make sure we havent already added the relationship
    auto& key1 = m_graph[m_relationvec.front()];
    for (auto& [key2, vec]: key1){
        for (auto& rel : vec) {
            if ((rel.relation == temp.relation) && (rel.seriescode == temp.seriescode) && (rel.threshold == temp.threshold)){
                std::cout << "failure" << "\n";
                return;
            }
        }

    }

    for (int i{0}; i < m_relationvec_size; i++){
        for (int j{0}; j < m_relationvec_size; j++){
            // Skipping the case where i = j because we dont want to make a relationship for a country with itself
            if (i != j){

                // Adding the relationship
                m_graph[m_relationvec[i]][m_relationvec[j]].push_back(temp);
                new_rel_or_not = true;
            }

        }
    }

    if (new_rel_or_not == true){
        std::cout << "success" << std::endl;
    } else {
        std::cout << "failure" << std::endl;
    }

}


void Tree::Tree_adjacent(std::string countrycode){

    
    std::string condition;

    for (int i{0}; i < m_items_stored; i++){
        if (m_list[i]->m_country_code == countrycode){
            condition = m_list[i]->m_country_name;
        } 
    }

        
    if (m_graph.find(condition) == m_graph.end()){
        std::cout << "failure" << std::endl;
        return;
    }

    if (m_graph[condition].size() == 0){
        std::cout << "none" << "\n";
        return;
    }
    
    auto& key1 = m_graph[condition];

    // How do I check if key1 is actutally storing the string I want or if it is empty?
    if (!key1.empty()){
        for (auto& [key2, rel]: key1) { 
            std::cout << key2 << " "; //Outputing the countries adjacent to the inputed country code
        }
        std::cout << "\n";
    } else {
        std::cout << "failure" << std::endl;
    }
    
}


void Tree::Tree_path(std::string cCode1, std::string cCode2) {
    std::string country1;
    std::string country2;

    // Finding the country names that correspond to the inputted country code
    for (int i{0}; i < m_items_stored; i++) {
        if (m_list[i]->m_country_code == cCode1) {
            country1 = m_list[i]->m_country_name;
        }
        if (m_list[i]->m_country_code == cCode2) {
            country2 = m_list[i]->m_country_name;
        }
    }

    // Doing a DFS V2 from country1 to country2
    std::stack<std::vector<std::string>> todo;
    std::set<std::string> visited;

    todo.push({country1}); //The country we are starting at "from"

    while (!todo.empty()) {
        std::vector<std::string> current_path = todo.top(); //assigning the top path in the stat to current path 
        todo.pop(); //poping the top path off of the stack

        std::string current_node = current_path.back(); // Getting the node at the back of the current path 

        // Country2 has been found "to"
        if (current_node == country2) {
            std::cout << "true" << "\n";
            return; 
        }

        // If current node isnt in "visited" then add the current node to visited
        if (visited.find(current_node) == visited.end()) {
            visited.insert(current_node);

            // Add each new path to the stack
            for (auto& neighbor : m_graph[current_node]) {
                std::vector<std::string> new_path = current_path;
                new_path.push_back(neighbor.first); //adding the adjacent countries to the end of current nodes path
                todo.push(new_path); //pushing the "new path" onto the stack
            }
        }
    }

    std::cout << "false" << std::endl;

}


void Tree::Tree_relationships(std::string cCode1, std::string cCode2){

    std::string country1;
    std::string country2;

    // Finding the country names that correspond to the inputted country code
    for (int i{0}; i < m_items_stored; i++) {
        if (m_list[i]->m_country_code == cCode1) {
            country1 = m_list[i]->m_country_name;
        }
        if (m_list[i]->m_country_code == cCode2) {
            country2 = m_list[i]->m_country_name;
        }
    }

    auto& key1 = m_graph[country1];

    if (key1.find(country2) != key1.end()){
        for (auto& relationvec : m_graph[country1][country2]) {
            std::cout << "(" << relationvec.seriescode << " " << relationvec.relation << " " << relationvec.threshold << ") ";
        }
        std::cout << "\n";
    } else {
        std::cout << "none" << std::endl;
    }

}

/*
---------------------------------------------Helper Functions-----------------------------------------------------------------------
*/


void Tree::Tree_populate_mean_array(std::string Series_Code){
    
    if (m_mean_values != nullptr){
        delete[] m_mean_values; //Delete m_values_array when making a new tree
    }
    m_mean_values = new double[m_items_stored];

    for (int i{0}; i < m_items_stored; i++){
        m_mean_values[i] = m_list[i]->series_code_mean(Series_Code); //Calculates the mean for each country
        if (m_mean_values[i] == -1){ //Dealing with invaid data
            m_mean_values[i] = 0;
        }
    }
    Tree_sortingalgorithm(m_mean_values);
}

void Tree::Tree_sortingalgorithm(double *array_to_sort){

    for (int i = 0; i < m_items_stored-1; i++){ //items -1 since we only want to go up to the second last array element
        bool check = false;
        for (int j = 0; j < m_items_stored-1-i; j++){ //subtract i since the end of the array is sorted
            if (array_to_sort[j] > array_to_sort[j + 1]){ //Moving the largers means to the end of the array
                // Swap values in array_to_sort
                double temp = array_to_sort[j];
                array_to_sort[j] = array_to_sort[j + 1];
                array_to_sort[j + 1] = temp;

                // Swap the corresponding countries linked list
                Linkedlist *tempList = m_list[j];
                m_list[j] = m_list[j + 1];
                m_list[j + 1] = tempList;

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
    for (int i{0}; i < m_items_stored; i++){

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

void Tree::Tree_find_recursive(Tree_Node *node, double mean, std::string operation){
    
    if (root == nullptr){ //empty tree
        //std::cout << "failure"; 
        return;
    }

    if (operation == "equal"){
        if (node == nullptr){ //Return if nullptr
            return; 
        } 
        
        if (node->m_right == nullptr && node->m_left == nullptr){ //We only print if we get to a leaf node
            if (std::abs(m_mean_values[node->m_min_mean_loc]-mean) <= 1E-3){ //Check if mean = mean values in the array within the 1E-3 tolorance

                for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                    m_relationvec.push_back(m_list[i]->m_country_name); 
                    m_relationvec_size++;
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
                    m_relationvec.push_back(m_list[i]->m_country_name); 
                    m_relationvec_size++;
                }
                return;
            }
        } 
        
        if (mean > node->m_max_range){ //If mean > max mean value all the values are less than

            for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                m_relationvec.push_back(m_list[i]->m_country_name); 
                m_relationvec_size++;
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
                    m_relationvec.push_back(m_list[i]->m_country_name); 
                    m_relationvec_size++;
                }
            }
            return;
        } 
        
        if (mean < node->m_min_range){ //If mean < min mean value then all mean values are greater than mean

            for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                m_relationvec.push_back(m_list[i]->m_country_name); 
                m_relationvec_size++;
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