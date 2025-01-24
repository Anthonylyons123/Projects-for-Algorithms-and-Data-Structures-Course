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

void Time_Series::LOAD (std::string file){ 
    delete[] arr_data; //Delete the old array's when a new file is loaded
    delete[] arr_year;

    m_arr_size = 2; //Reseting m_arr_size to 2 
    m_count = 0; //Reseting m_count to 0

    arr_data = new double[m_arr_size]; //Making new array's and initalizing them to size 2
    arr_year = new int[m_arr_size];

    int year{1960}; //The first year is 1960

    std::string data_string;

    std::ifstream file1 (file); //opens the file                                      
    std::getline (file1, c_name, ','); //Makes a string with Country Name 
    std::getline (file1, c_cf, ','); //Makes a string with Country Code
    std::getline (file1, s_name, ','); //Makes a string with Series Name
    std::getline (file1, s_code, ','); //Makes a string with Series Code

    while (std::getline (file1, data_string, ',')){ //Takes the data and inputs it into the dynamically allocated data and year array
        /*
            CITATION:

            I got the std::stod command and c_str() from chatgbt
        */
       
        double data = std::stod(data_string.c_str()); //Converts the string to double
        does_arr_need_resized(); 
        
        if (data != -1){ //This is filling up the array with valid data
        /*
            CITATION:

            I got the std::stod command and c_str() from chatgbt
        */
            arr_data[m_count] = std::stod(data_string.c_str()); //Converts string to float
            arr_year[m_count] = year;
            ++m_count;
        }
        ++year; //Increment the year every loop even if there is no valid data
    }

}

void Time_Series::PRINT(){
    if(m_count == 0){ //Checks if there is data to print
        std::cout << "failure" << std::endl;
        return;
    }
    for (int i{0}; i < m_count; i++){ //Print the year and data in this form (year, data)
        std::cout<< "(" <<arr_year[i] << "," << arr_data[i] << ") ";
    }
    std::cout << std::endl;
}

void Time_Series::ADD(double data, int year){
    int arr_loc{0};
    if (data == -1){ //Check if data is invalid
        std::cout << "failure" << std::endl;
        return;
    } else {
        if (year < arr_year[0]){ //If year is smaller than the smallest entry put new data at start of array
            arr_shift(arr_loc, year, data);
            m_count++;
            
        } else if (year > arr_year[m_count-1]){ //If year is bigger than the biggest entry put new data at end of array
            arr_data[m_count] = data;
            arr_year[m_count] = year;
            m_count++;

        } else { //Else the year is somewhere in the middle
            for (int i{0}; i < m_count; i++){ //Finding the loction to input the new data & year
                if (year < arr_year[i]){
                    arr_loc = i;
                    break;
                }
                if (year == arr_year[i]){ //Checking if the added year already has valid data
                    std::cout << "failure" << std::endl;
                    return; 
                }
            }

            arr_shift(arr_loc, year, data); //Using helper function to input the new data in the array's and shift the element to the correct loction
            m_count++;

        }
        does_arr_need_resized(); //Checks if array sizes need changed
        std::cout << "success" << std::endl;
    }
}

void Time_Series::UPDATE(double data, int year){
    int temp_loc{-1};

    for (int i{0}; i < m_count; i++){ //Looping through year array to check if this year already has valid data
        if (arr_year[i] == year){
            temp_loc = i;
            break;
        }
    }

    if (temp_loc == -1){ //If temp_loc doesn't change from -1 in the for loop return because the "new" year doesn't have valid data to update
        std::cout << "failure" << std::endl; //or m_count == 0
        return; 
    }

    if (data == -1){ //Checking if the "new" data is invaild
        for (int i{temp_loc}; i < m_count-1; i++){ //If the "new" data is invailid I remove it from the array's
            arr_data[i] = arr_data[i+1]; 
            arr_year[i] = arr_year[i+1];
        }
        m_count--; //Decrement the count since there is one less item in the arrays

    } else { //Updates the data with the "new" data if the data is vaild
        arr_data[temp_loc] = data;
    }
    does_arr_need_resized(); //Check if array needs to be resized
    std::cout << "success" << std::endl;
}

double Time_Series::mean(){ 
    double mean_value{0};

    if (m_count == 0){ //Check if there is any data
        std::cout << "failure" << std::endl;
        return 0; //If no data return 0
    } else {
        for (int i{0}; i < m_count; i++){
            mean_value += arr_data[i]; //Geting the total data
        }
        mean_value = (mean_value/m_count); //Calculating the mean
        std::cout << "mean is " << mean_value << std::endl; //If there is data output that result
    }
    return 0;
}
    
bool Time_Series::is_monotonic(){ 

    bool up{true}; 
    bool down{true};

    if (m_count == 0){ //Returns false if there is no data
        std::cout << "failure" << std::endl;
        return false;
    }

    for (int i{1}; i < m_count; i++){
        if (arr_data[i] > arr_data[i-1]){ //Checks if data is increasing
            down = false; //If data is increasing then set down to false
        }
        if (arr_data[i] < arr_data[i-1]){ //Checks if data is decreasing
            up = false; ///If data is decreasing then set up to false
        }
    }

    if (up||down == true){                                          
        std::cout << "series is monotonic" <<std::endl;     //If one of up/down is true then the data is monotonic
        return true;                                        //but if both increasing/decreasing are false then at somepoint the data was both 
    } else {                                                //increasing and decreasing and is therefore not monotonic
        std::cout << "series is not monotonic" <<std::endl; 
        return false;
    }

}
    
void Time_Series::best_fit(double &m, double &b){
    // Year is x
    // Data is y

    double xy_sum{0}; //sum of x*y
    double x_sum{0}; //sum of x
    double y_sum{0}; //sum of y
    double xx_sum{0}; //sum of x^2

    if (m_count == 0){ //If count is 0 return
        std::cout << "failure" << std::endl;
        return;
    }
    for (int i{0}; i < m_count; i++){ //Finding values for the sum of (x*y), x, y, and (x*x)
        xy_sum += (arr_year[i]*arr_data[i]);
        x_sum += arr_year[i];
        y_sum += arr_data[i];
        xx_sum += (arr_year[i]*arr_year[i]);
    }
    m = ((m_count*xy_sum)-(x_sum*y_sum))/((m_count*xx_sum)-(x_sum*x_sum)); //Using the provided formulas
    b = (y_sum-(m*x_sum))/m_count; 

    std::cout << "slope is " << m << " intercept is " << b <<std::endl;
}


// Helper function definitions
void Time_Series::arr_shift(int temp_val, int year, double data){
    for (int i{m_count-1}; i >= temp_val; i--){ //Loop through the array and shift everything over one array index higher
        arr_data[i+1] = arr_data[i];
        arr_year[i+1] = arr_year[i];
    }
    //Sets the data from ADD function to the loction temp_val loction
    arr_data[temp_val] = data; 
    arr_year[temp_val] = year;
}

void Time_Series::arr_resize(){
    double *temp_data = new double[m_arr_size]; //Create temp data array
    int *temp_year = new int[m_arr_size]; //Create temp year array

    for (int i{0}; i < m_count; i++){ //populating temp arrays with data & years
        temp_data[i] = arr_data[i];
        temp_year[i] = arr_year[i];
    }
    delete[] arr_data; 
    delete[] arr_year;

    arr_data = temp_data; //assigning arr_data pointer to the new temp_data address
    arr_year = temp_year; //assigning arr_year pointer to the new temp_data address
    temp_data = nullptr;
    temp_year = nullptr;
}

void Time_Series::does_arr_need_resized(){
        if (m_arr_size == m_count){ //Check if array size needs to be 2x
            m_arr_size = 2*m_arr_size;
            arr_resize(); //resize the array
        } 

        if (m_arr_size == m_count/4){ //Check if array size needs to be 1/4x
            m_arr_size = m_arr_size/4;
            arr_resize(); //resize the array
        }
}