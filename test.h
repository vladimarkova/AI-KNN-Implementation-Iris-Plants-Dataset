#include <iostream>
#include <ratio>
#include <chrono>
#include "algorithm.h"
using namespace std;


void test() {
    char response = '1';

    while(response != '0') {
        using namespace std::chrono;
        high_resolution_clock::time_point t1 = high_resolution_clock::now(); 

        KNN();

        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double, std::milli> time_span = t2 - t1; 

        cout << "Time in miliseconds for KNN classification was: " << time_span.count() << " milliseconds." << endl << endl;
        
        print_result_of_single_entry_classification();

        cout << "Enter 0 to stop tests or 1 to continue: " << endl << endl;
        cin >> response;
    }
}