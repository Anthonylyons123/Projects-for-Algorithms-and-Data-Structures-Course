//#pragma once
#include <iostream>

#ifndef time_series_class
#define time_series_class

class Time_Series {
    public:
    
    Time_Series(); // Constructor
    ~Time_Series(); // Destructor

    // Function Declarations
    void LOAD(std::string file);
    void PRINT();
    void ADD(double data, int year);
    void UPDATE();
    double mean(); 
    bool is_monotonic();
    void best_fit(double &m, double &b);
    void EXIT();

    private:
    // DAA for years
    int* arr_year;
    
    //DAA for the data
    double* arr_data;

    int arr_size{2}; //Size of array
    int count{0}; //Number of elements in the array
    
    //Series Name & Code
    std::string s_name;
    std::string s_code;

    //Helper functions
    void arr_shift(int temp_val, int year, double data);
    void arr_resize();
}; 



#endif