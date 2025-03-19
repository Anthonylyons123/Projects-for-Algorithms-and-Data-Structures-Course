#include <iostream>
#include <sstream>
#include <fstream>

#include "tree.hpp"

Tree::Tree(){

}

Tree::~Tree(){
// Delete m_list array  
for (int i{0}; i < 512; i++){
    if (m_list[i] != nullptr) delete m_list[i];
}

if (m_mean_values != nullptr) delete[] m_mean_values;
Tree_post_order_delete(root);

if (m_sorted_mean_index != nullptr) delete[] m_sorted_mean_index;
}

void Tree::Tree_load(std::string filename){
    std::string line;
    std::string temp_country {" "};
    //Linkedlist *temp_arr[1];
    //int i{0};

    std::ifstream info_file (filename); 
    while (std::getline(info_file, line)){ //read each line of csv file
        std::stringstream country_name(line);
        std::string firstword;
        std::getline(country_name, firstword, ',');
        std::string secondword;
        std::getline(country_name, secondword, ',');

        if (firstword != temp_country) { //New country

            // Check if that country is already stored in the hash table
            if (Tree_search_table(secondword, false) == -1){
                
                m_temp_arr = secondword;

                int index = Tree_prime_Hash();
                m_list[index] = new Linkedlist; // Creating new memory inside the array
                m_list[index]->load(firstword, filename); //Filling up the array
                m_list[index]->m_country_name = firstword; 
    
                m_stored_or_not[index] = true;
                
                m_items_stored++; //Keep count of how many linked lists are stored in the array
            }

            temp_country = firstword; //temp_country is assigned the string in firstword so that we don't duplicate countries linked lists

        }
    }

}

void Tree::Tree_list(std::string Country_Name){
    for (int i = 0; i < 512; i++) {
        if (m_list[i] != nullptr){
            if (m_list[i]->m_country_name == Country_Name) {
                m_list[i]->list();
            }
        }
    }
}
void Tree::Tree_range(std::string Series_Code){
    double min_mean{0};
    double max_mean{0};
    double temp_mean{0};

    for (int i{0}; i < 512; i++){ //Loop through each countries linked list
        
        if (m_list[i] != nullptr){ // If its nullptr, there is nothing in the array

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
    
    }
    std::cout << min_mean << " " << max_mean <<std::endl;
}

void Tree::Tree_build(std::string Series_Code){
    Tree_post_order_delete(root); //Delete previos tree
    m_items_del = 0; //Reset this value every time a tree is being built from a new series code

    m_series_code = Series_Code; //Will need this for the deleting the tree

    for (int i{0}; i < 512; i++){ //Moving all pointers in m_list to m_list_del
        m_list_del[i] = m_list[i];
    }

    Tree_populate_mean_array(Series_Code); // Sorting the mean values
    root = new Tree_Node;
    Tree_build_recursive(Series_Code, root, m_mean_values[m_sorted_mean_index[0]], m_mean_values[m_sorted_mean_index[m_items_stored-m_items_del-1]]);
    //print(root);  

}
void Tree::Tree_find(double mean, std::string opperation){
    Tree_find_recursive(root, mean, opperation); //Pass the root, mean, and operation into the recursive function
    std::cout << "\n";
}

void Tree::Tree_delete(std::string Country_Name){
    Tree_delete_recursive(m_series_code, Country_Name, true); //Pass the root, and country name into the recursive function
}
void Tree::Tree_limits(std::string condition){
    Tree_limits_recursive(root, condition); //Pass the root, and condition into the recursive function
    std::cout << "\n";
}

void Tree::Tree_lookup(std::string countrycode){
    Tree_search_table(countrycode, true);
}
void Tree::Tree_remove(std::string countrycode){
    
    int remove = Tree_search_table(countrycode, false);

    if (remove == -1){
        std::cout << "failure" << "\n";
        return;
    } else {

        //m_mean_values[remove] = -100; // Set the mean value at this location to -100 so we know nullptr is stored at this location in m_list

        Tree_delete_recursive(m_series_code, m_list[remove]->m_country_name, false); // Delete the tree
        delete m_list[remove];
        m_list[remove] = nullptr; //Set this array location to nullptr;

        std::cout << "success" << "\n";
        --m_items_stored;

    }

}
void Tree::Tree_insert(std::string countrycode, std::string filename){

    // If Tree_search_table returns -1 then the country we are trying to add isnt already stored
    if (Tree_search_table(countrycode, false) == -1){

        std::string line;
        std::string temp_country {" "};
    
        std::ifstream info_file (filename); 
        while (std::getline(info_file, line)){ //read each line of csv file
            std::stringstream country_name(line);
            std::string firstword;
            std::getline(country_name, firstword, ',');
            std::string secondword;
            std::getline(country_name, secondword, ',');
            
            if (secondword == countrycode){
    
                m_temp_arr = secondword;
                int index = Tree_prime_Hash();
    
                m_list[index] = new Linkedlist; // Creating new memory inside the array
                m_list[index]->load(firstword, filename); //Filling up the array
                m_list[index]->m_country_name = firstword; 
    
                m_stored_or_not[index] = true;
                
                m_items_stored++; //Keep count of how many linked lists are stored in the array
                
                std::cout << "success" << std::endl;

                return;
    
            }
    
        }
    } else {
        std::cout << "failure" << std::endl;
    }

}



//------------------------------------------- Helper Functions --------------------------------------------------------//
void Tree::Tree_populate_mean_array(std::string Series_Code){
    
    if (m_mean_values != nullptr){
        delete[] m_mean_values; //Delete m_values_array when making a new tree
    }
    m_mean_values = new double[512];

    for (int i{0}; i < 512; i++){
        if (m_list_del[i] != nullptr){ //If array at i is nullptr dont do anything

            m_mean_values[i] = m_list_del[i]->series_code_mean(Series_Code); //Calculates the mean for each country
            if (m_mean_values[i] == -1){ //Dealing with invaid data
                m_mean_values[i] = 0;
            }

        } else {
            m_mean_values[i] = -100; //Store a value of -100 if the list array at i stores nullptr (the location is empty in the hash table)
        }
    }

    Tree_sortingalgorithm();
    
}

void Tree::Tree_sortingalgorithm(){

    if (m_sorted_mean_index != nullptr){
        delete[] m_sorted_mean_index; //Delete m_values_array when making a new tree
    }
    m_sorted_mean_index = new int[m_items_stored-m_items_del];
    
    // Initialize index array
    int index{0};
    for (int i = 0; i < 512; i++) {
        //std::cout << m_mean_values[i] << ", ";
        if (m_mean_values[i] != -100){ //If mean values at i is -100 that means the list array has a nullptr so we dont want to store that index
            m_sorted_mean_index[index] = i;  
            index++;
        }
    }

    // Selection Sort on the index array based on values[]
    for (int i = 0; i < m_items_stored-m_items_del - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < m_items_stored-m_items_del; j++) {
            
            if (m_mean_values[m_sorted_mean_index[j]] < m_mean_values[m_sorted_mean_index[min_index]]) {
                min_index = j;
            }

        }

        // Manually swap elements of indices array (without a swap function)
        int temp = m_sorted_mean_index[i];
        m_sorted_mean_index[i] = m_sorted_mean_index[min_index];
        m_sorted_mean_index[min_index] = temp;
    }

}

void Tree::Tree_find_mean_loaction(Tree_Node *Node, double min_mean, double max_mean){
    
    //Finding the array index for the minimum mean and maximum mean value
    for (int i{0}; i < m_items_stored-m_items_del; i++){

        if (m_mean_values[m_sorted_mean_index[i]] < min_mean){
            Node->m_min_mean_loc = i+1;
        }
        if (m_mean_values[m_sorted_mean_index[i]] <= max_mean){
            Node->m_max_mean_loc = i;
        }
    }
}

Tree_Node* Tree::Tree_build_recursive(std::string Series_Code, Tree_Node *node, double min_mean, double max_mean){

    Tree_find_mean_loaction(node, min_mean, max_mean); //Finding array index for min and max mean value
    node->m_min_mean = min_mean; 
    node->m_max_mean = max_mean;

    // Here the second part of the or no longer works... I dont think
    // (m_mean_values[node->m_max_mean_loc]-m_mean_values[node->m_min_mean_loc] <= 1E-3) || ((node->m_max_mean_loc - node->m_min_mean_loc)+1 <= 1)
    if ((m_mean_values[m_sorted_mean_index[node->m_max_mean_loc]]-m_mean_values[m_sorted_mean_index[node->m_min_mean_loc]] <= 1E-3)){ // If the difference between min and max mean in less than 0.001 they can go in the same leaf node
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
            
            if (std::abs(m_mean_values[m_sorted_mean_index[node->m_min_mean_loc]]-mean) <= 1E-3){ //Check if mean = mean values in the array within the 1E-3 tolorance
                

                // Looping through the list array and outputing the countries
                for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                    std::cout << m_list_del[m_sorted_mean_index[i]]->m_country_name << " ";
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
            if (m_mean_values[m_sorted_mean_index[node->m_min_mean_loc]] < mean){ //If the mean values in the array < inputed mean value print the countries
                
                // Looping through the list array and outputing the countries
                for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                    std::cout << m_list_del[m_sorted_mean_index[i]]->m_country_name << " ";
                }
                return;
            }
        } 
        
        if (mean > node->m_max_mean){ //If mean > max mean value all the values are less than
                
            // Looping through the list array and outputing the countries
            for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                std::cout << m_list_del[m_sorted_mean_index[i]]->m_country_name << " ";
            }
            return;
        } 
        
        if (mean < node->m_min_mean){ //If mean < min mean value, there is no data less than mean
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
            if (m_mean_values[m_sorted_mean_index[node->m_min_mean_loc]] > mean){ //If the mean values in the array > inputed mean value print the countries
                
                // Looping through the list array and outputing the countries
                for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                    std::cout << m_list_del[m_sorted_mean_index[i]]->m_country_name << " ";
                }
            }
            return;
        } 
        
        if (mean < node->m_min_mean){ //If mean < min mean value then all mean values are greater than mean
                
            // Looping through the list array and outputing the countries
            for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                std::cout << m_list_del[m_sorted_mean_index[i]]->m_country_name << " ";
            }
            return;
        }
        
        if (mean > node->m_max_mean){ //If m > max mean value, then there are no mean values < mean so return
            return;
        }

        Tree_find_recursive (node->m_left, mean, operation);
        Tree_find_recursive (node->m_right, mean, operation);
        return; 
    }

}

void Tree::Tree_delete_recursive(std::string series_code, std::string country, bool func_call){
    
    // If func_call is true we want the couts otherwise dont print anything

    bool check = false;
    if (root == nullptr && func_call == true){ //empty tree
        std::cout << "failure" << "\n";
        return;
    } else if (root == nullptr && func_call == false){
        return;
    }

    for (int i{0}; i < m_items_stored-m_items_del; i++){
        if (m_list_del[m_sorted_mean_index[i]]->m_country_name == country){ //Checking if the country being removed is in that node
            for (int j{i}; j < m_items_stored-m_items_del-1; j++){ //If the country is in that node shift all of the entries in the mean and m_list_del array left 1
                // m_list_del[m_sorted_mean_index[j]] = m_list_del[m_sorted_mean_index[j+1]];
                // m_mean_values[m_sorted_mean_index[j]] = m_mean_values[m_sorted_mean_index[j+1]];

                m_sorted_mean_index[j] = m_sorted_mean_index[j+1];
            }
            m_items_del++; //Count how many countries have been deleted from the tree
            check = true;
        }
    }
    if (func_call == true){
        if (check == true){    
            std::cout << "success" << "\n";
        } else {
            std::cout << "failure" << "\n";
        }
    }

    Tree_post_order_delete(root); //delete the tree
    Tree_build_recursive(series_code, root = new Tree_Node, m_mean_values[m_sorted_mean_index[0]], m_mean_values[m_sorted_mean_index[m_items_stored-m_items_del-1]]); //Rebuild the tree without the country we deleted

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
            for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                std::cout << m_list_del[m_sorted_mean_index[i]]->m_country_name << " ";
            }
        }
        
        Tree_limits_recursive (node->m_left, condition); //Recurse left

    } else if (condition == "highest"){
        if (node == nullptr){
            return;
        } 
        
        if (node->m_right == nullptr){ //Print countries in right most node
            for (int i{node->m_min_mean_loc}; i <= node->m_max_mean_loc; i++){
                std::cout << m_list_del[m_sorted_mean_index[i]]->m_country_name << " ";
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


int Tree::Tree_prime_Hash(){

    //int W = ((m_temp_arr[0]->m_country_code[0]-'A')*26*26)+((m_temp_arr[0]->m_country_code[1]-'A')*26)+(m_temp_arr[0]->m_country_code[2]-'A'); //Calculating W in the provided formula
    
    int W = ((m_temp_arr[0]-'A')*26*26)+((m_temp_arr[1]-'A')*26)+(m_temp_arr[2]-'A'); //Calculating W in the provided formula
    int H1 = W % 512;

    // Check if there is something stored in the list array at hash 1 value, if not return that value
    if (m_list[H1 % 512] == nullptr){
        //std::cout << H1 % 512 << "\n";
        return H1 % 512; 

    } else {
        // Already one value stored at H1 value, so calculate the seconded hash value
        int H2 = Tree_sec_Hash(W);
        int i{1};

        // Keep adding 1 to i in the overall hash function equation until we an array location with nothing in it
        while (true){
            //std::cout << "stuck in loop ";
            if (m_list[(H1 + i*H2) % 512] == nullptr){
                //std::cout << (H1 + i*H2) % 512 << "\n";
                return (H1 + i*H2) % 512;
            }
            i++;
        }
    }    


}

int Tree::Tree_sec_Hash(int W){
    
    int result = (W/512) % 512;

    if (result % 2 == 0){
        return result+1; //hash two is even so return result + 1
    } else {
        return result; //hash two is odd so return result
    }

}

int Tree::Tree_search_table(std::string countrycode, bool check){

    // If check is true, we are doing the lookup function (searching the hash table and then printing index and # searches)
    // If check is false we are searching the table for other functions

    int i = 0; //This is the number of searches and used to calculate the hash values
    int W = ((countrycode[0]-'A')*26*26)+((countrycode[1]-'A')*26)+(countrycode[2]-'A'); //Calculating W in the provided formula
    int H1 = W % 512;

    // If m_stored_or_not at H1 is false, there has never been anything stored at that array location, therefore stop searching the array
    if (m_stored_or_not[H1] == false){

        if (check == true) std::cout << "failure" << "\n";
        return -1;
    
    } 
    
    if (m_list[H1] != nullptr){ // Make sure we dont access nullptr
        if (m_list[H1]->m_country_code == countrycode){ // If true we found the matching country code
            
            if (check == true) std::cout << "index " << H1 << " searches " << i+1 << "\n";
            return H1;
        }
        
    } 
            
        i++;
        int H2 = Tree_sec_Hash(W);
    
        while (true){

            // If m_stored_or_not at H1 is false, there has never been anything stored at that array location, therefore stop searching the array
            if (m_stored_or_not[(H1 + i*H2) % 512] == false){

                if (check == true)  std::cout << "failure" << "\n";
                return -1;

            } 
            
            if (m_list[(H1 + i*H2) % 512] != nullptr){ // Make sure we dont access nullptr

                if (m_list[(H1 + i*H2) % 512]->m_country_code == countrycode){ // If true we found the matching country code
                    if  (check == true) std::cout << "index " << (H1 + i*H2) % 512 << " searches "<< i+1 <<"\n";
                    return (H1 + i*H2) % 512;
                }
            
            }
            i++;
    
        }
    
}

bool Tree::Tree_is_CCode_stored(std::string countrycode){

    for (int i{0}; i < 512; i++){
        if (m_stored_or_not[i] == true){
            if (m_list[i]->m_country_code == countrycode){
                return true;
            }
        }
    }

    return false;

}