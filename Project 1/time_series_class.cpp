#include <iostream>
#include <sstream>
#include <fstream>

#include "time_series_class.h"

Time_Series::Time_Series(){
    arr_year = new int[2];
    arr_data = new double[2];
}

Time_Series::~Time_Series(){
    delete[] arr_year;
    arr_year = nullptr;

    delete[] arr_data;
    arr_data = nullptr;
}
    

//Need to make an edge case for "size" never goes smaller than 2
void Time_Series::LOAD (std::string file){ 
    int year{1960};

    std::string c_n;
    std::string c_cf;
    std::string data_string;

    std::ifstream file1 ("lab1_canada.csv"); //opens the file                                      
    std::getline (file1, c_n, ','); //Makes a string with Country Name 
    std::getline (file1, c_cf, ','); //Makes a string with Country Code
    std::getline (file1, s_name, ','); //Makes a string with Series Name
    std::getline (file1, s_code, ','); //Makes a string with Series Code


    while (std::getline (file1, data_string, ',')){ //Takes the data and inputs it into the dynamically allocated data array
            
        //I got the std::stod command and c_str() from chatgbt
        double data = std::stod(data_string.c_str()); //Converts the string to double

        if (arr_size == count){ //Check if array size needs to be 2x
            arr_size = 2*arr_size;

            //Make a function for this....
            double *temp_data = new double[arr_size]; //Create temp data array
            int *temp_year = new int[arr_size]; //Create temp year array

            for (int i{0}; i < count; i++){ //populating temp arrays with data & years
                temp_data[i] = arr_data[i];
                temp_year[i] = arr_year[i];
            }

            delete[] arr_data; 
            delete[] arr_year;

            arr_data = temp_data; //assigning arr_data address to the new temp_data address
            arr_year = temp_year; //assigning arr_year address to the new temp_data address
            temp_data = nullptr;
            temp_year = nullptr;

        } 
            
        if (arr_size == count/4){ //Check if array size needs to be 1/4x
            arr_size = arr_size/4;

            //Make a function for this
            double *temp_data = new double[arr_size]; //Create temp data array
            int *temp_year = new int[arr_size]; //Create temp year array

            for (int i{0}; i < count; i++){ //populating temp arrays with data & years
                temp_data[i] = arr_data[i];
                temp_year[i] = arr_year[i];
            }

            delete[] arr_data; 
            delete[] arr_year;

            arr_data = temp_data; //assigning arr_data address to the new temp_data address
            arr_year = temp_year; //assigning arr_year address to the new temp_data address
            temp_data = nullptr;
            temp_year = nullptr;
        }

        if (data != -1){ //This is filling up the array with valid data

            //I got the std::stod command and c_str() from chatgbt
            arr_data[count] = std::stod(data_string.c_str()); //Converts string to float
            arr_year[count] = year;
            ++count;
        }
        ++year;
    }

}

void Time_Series::PRINT(){
    for (int i{0}; i < count; i++){
        std::cout<< "(" <<arr_year[i] << "," << arr_data[i] << ")";
    }
}

void Time_Series::ADD(double data, int year){
    int arr_loc{0};
    if (data == -1){ //Check if data is invalid
        return;
    } else {
        if (year < arr_year[0]){ //If year is smaller than the smallest entry put at start of array

            if (arr_data[arr_loc] == -1){
                arr_shift(arr_loc, year, data);
            } else {
                return;
            }

        } else if (year > arr_year[count]){ //If year is bigger than the biggest entry put at end of array
                
            if (arr_data[arr_loc] == -1){
                arr_shift(arr_loc, year, data);
            } else {
                return;
            }

        } else {

            for (int i{0}; i < count; i++){ //Finding the loction to input the new data & year
                if (year == arr_year[i]){
                    arr_loc = i;
                }
            break;
            }

            if (arr_data[arr_loc] == -1){
                arr_shift(arr_loc, year, data);
            } else {
                return;
            }

        }

    }
}
double Time_Series::mean(){ //Computes mean
    
    double mean_value{0};

    if (arr_data[0] == 0){ //Check if there is any data
        return 0; //If no data return 0
    } else {
        for (int i{0}; i < count; i++){
            mean_value += arr_data[i]; //Geting the total data
        }
        mean_value == (mean_value/count); //Calculating the mean
        return mean_value; //If there is data return mean
    }
    return 0;
}
    
bool Time_Series::is_monotonic(){ //Is the array sorted

    return true;

};
    
void Time_Series::best_fit(double &m, double &b){

        
}

void Time_Series::arr_shift(int temp_val, int year, double data){
    for (int i{count}; i > temp_val; i--){
        // 
         // Need to check if 
        //
        arr_data[i+1] = arr_data[i];
        arr_year[i+1] = arr_year[i];
    }

    arr_data[temp_val] = data;
    arr_year[temp_val] = year;
}

void Time_Series::arr_resize(){
    double *temp_data = new double[arr_size]; //Create temp data array
    int *temp_year = new int[arr_size]; //Create temp year array

    for (int i{0}; i < count; i++){ //populating temp arrays with data & years
        temp_data[i] = arr_data[i];
        temp_year[i] = arr_year[i];
    }
    delete[] arr_data; 
    delete[] arr_year;

    arr_data = temp_data; //assigning arr_data address to the new temp_data address
    arr_year = temp_year; //assigning arr_year address to the new temp_data address
    temp_data = nullptr;
    temp_year = nullptr;
}