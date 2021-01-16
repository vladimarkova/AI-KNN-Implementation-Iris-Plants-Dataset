#include <iostream>
#include <ratio>
#include <chrono>
#include "algorithm.h"
using namespace std;


void test() {
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now(); 

    // isolated_tests();
    KNN();

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double, std::milli> time_span = t2 - t1; 

    cout << "Time in miliseconds for KNN classification was: " << time_span.count() << " milliseconds." << endl << endl;
}