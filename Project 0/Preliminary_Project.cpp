#include <iostream>
#include <sstream>
#include <fstream>

int main();

int main(){

    std::ifstream file("project0_test_data.csv");
    std::string string;
    std::string string2;
    
   /* while (std::getline (file, string)){
        std::stringstream sstream(string);
        std::cout<< string;
            while (std::getline(sstream, string2, ',')){
                std::cout<< string2 <<std::endl;
            }

    } */

   std::getline(file, string, ',');
   std::getline(file, string2, ',');

   std::cout<< string <<std::endl;
   std::cout<< string2 <<std::endl;
    
}

