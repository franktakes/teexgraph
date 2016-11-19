/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 * 
 * The Timer class measures wall clock execution time, respecting paralellism.
 * Create the object Timer T and run T.click() to see time since the last click.
 */

#ifndef TIMER_H
#define TIMER_H

class Timer {
  public:
    Timer();
    void click();
  private:
    double current;

    double now();
    void restart();
};

#endif /* TIMER_H */

