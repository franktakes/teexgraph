/*
 * teexgraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 * 
 * Example main-file showing how to use teexgraph
 */

#include "BDGraph.h"
#include "Timer.h"
using namespace std;

int main(const int argc, const char* argv[]) {
	assert(argc > 1); // catch no input graph given
	Timer T; // timer object to measure loading time
	T.click(); // start timer
	BDGraph G; // create graph object
	if(!G.loadDirected(argv[1])) // load network edge list from file argv[1]
		return 1;
	T.click(); // see loading time and start measuring execution time
	G.computeWCC(); // find weakly connected components
	G.stats(); // show some statistics of the network	
	T.click();	
	G.computeSCC(); // find strongly connected components
	T.click(); // show and restart timer
	G.stats(); // show some statistics of the network
	T.click(); // report diameter computation time
	return 0;
}

