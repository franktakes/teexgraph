/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 * 
 * The Timer class measures wall clock execution time, respecting paralellism.
 * Create the object Timer T and run T.click() to see time since the last click.
 */

#include "Timer.h"
#include "omp.h"
#include <iostream>
using namespace std;
Timer::Timer() {
    current = -1;
}
void Timer::click() {
    if(current != -1)
        cerr << "Time passed: " << now() - current << " sec." << endl;
    restart();
}
double Timer::now() {
    #ifdef _OPENMP
    return omp_get_wtime();
    #endif
    return(double) clock() / (double) CLOCKS_PER_SEC;
}
void Timer::restart() {
    current = now();
}

