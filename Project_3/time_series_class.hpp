#include <iostream>

#ifndef time_series_class
#define time_series_class

class Time_Series {
    
    public:

    private:
    Time_Series(); // Constructor
    ~Time_Series(); // Destructor

    // Function Declarations
    void LOAD(std::string tempstring1);
    void PRINT();
    void ADD(double data, int year);
    void UPDATE(double data, int year);
    double mean(); 
    bool is_monotonic();
    void best_fit(double &m, double &b);
    

    // Member Variables
    int* arr_year; //DAA for years
    double* arr_data; //DAA for the data
    int m_arr_size{2}; //Size of array
    int m_count{0}; //Number of elements in the array
    std::string m_s_name; //Series Name
    std::string m_s_code; //Series Code
    std::string m_c_name; //Country Name
    std::string m_c_code; //Country Code Feild

    //Helper functions
    void arr_shift(int temp_val, int year, double data);
    void arr_resize();
    void does_arr_need_resized();

    //New stuff for Project 2
    Time_Series *m_next;
    friend class Linkedlist;
    friend class Tree;
    
}; 

#endif