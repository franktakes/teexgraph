/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 *
 * The Timer class measures wall clock execution time, respecting paralellism.
 * Create the object Timer T and run T.click() to see time since the last click.
 */

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
  public:
    Timer() = default;
    void click();
  private:
    std::chrono::high_resolution_clock::time_point current;
    bool used = false;

    std::chrono::high_resolution_clock::time_point now();
    void restart();
};

#endif /* TIMER_H */
