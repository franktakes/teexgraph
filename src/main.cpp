/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 * 
 * Example main-file showing how to use teexGraph
 */

#include "BDGraph.h"
#include "Timer.h"
using namespace std;

#include "examples.hpp"
    
// main function
int main(const int argc, const char* argv[]) {

    // catch no input graph given in command line parameter
    assert(argc > 1);

    // timer object to measure loading and computation time
    Timer T;
    T.click();

    // create and load graph from file. pass an int >= nodecount 
    // to optimize memory usage or if you want to have a nodecount > 10M nodes
    BDGraph G;
    if(!G.loadUndirected(argv[1]))
        return 1;

    // detect weakly connected components
    G.computeWCC();

    // show some statistics of the network	
    stats(G);
    T.click();

    // compute the diameter of the largest WCC of this network
    cout << G.diameterBD() << endl;
    T.click();
    return 0;
}

