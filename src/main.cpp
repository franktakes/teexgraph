/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 * 
 * Example main-file showing how to use teexGraph
 */

#include "Graph.h"
#include "Timer.h"
using namespace std;

int main(const int argc, const char* argv[]) {

    // catch no input graph given in command line parameter
    assert(argc > 1);

    // timer object to measure loading and computation time
    Timer T;
    T.click();

    // create and load graph from file
    Graph G;
    if(!G.loadUndirected(argv[1]))
        return 1;

    // detect weakly connected components
    G.computeWCC();

    // show some statistics of the network	
    G.stats();
    T.click();

    // compute the distance distribution (APSP) in parallel
    G.distanceDistribution(WCC, 1);
    T.click();
    return 0;
}

