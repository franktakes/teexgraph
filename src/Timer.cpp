/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 *
 * The Timer class measures wall clock execution time, respecting paralellism.
 * Create the object Timer T and run T.click() to see time since the last click.
 */

#include <teexgraph/Timer.h>

#include "omp.h"
#include <iostream>

using namespace std;

// show and restart timer
void Timer::click() {
    if(used){
        using namespace std::chrono;
        const auto current_time = now();
        const auto time_diff = duration_cast<duration<double>>((current_time-current));
        cerr << "Time passed: " << time_diff.count() << " sec." << endl;
    }
    used = true;
    restart();
}

// get now time
std::chrono::high_resolution_clock::time_point Timer::now() {
    return std::chrono::high_resolution_clock::now();
}

// restart timer
void Timer::restart() {
    current = now();
}
