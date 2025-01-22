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
    void UPDATE(double data, int year);
    double mean(); 
    bool is_monotonic();
    void best_fit(double &m, double &b);
    
    private:
    // Member Variables
    int* arr_year; //DAA for years
    double* arr_data; //DAA for the data
    int m_arr_size{2}; //Size of array
    int m_count{0}; //Number of elements in the array
    std::string s_name; //Series Name
    std::string s_code; //Series Code

    //Helper functions
    void arr_shift(int temp_val, int year, double data);
    void arr_resize();
    void does_arr_need_resized();
}; 

#endif