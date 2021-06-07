/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 *
 * Examples of how to use the code
 */

#include "examples.h"
#include <iostream>
using namespace std;

// output how often each distance value occurs
void showDistanceDistribution(Graph & G) {
	vector<long> distances;
	// second parameter "1.0" can be used to state a sampling percentage
    if(G.wccComputed())
    	distances = G.distanceDistribution(Scope::LWCC, 1.0);
    else
    	distances = G.distanceDistribution(Scope::FULL, 1.0);
    G.printList(distances);
}

// show basic stats of the network
void stats(Graph & G) {
    const int LEFTWIDTH = 40;

    // the following metrics are linear in computation time and "free" to show"
    clog << setw(LEFTWIDTH) << "Nodes n: " << G.nodes(Scope::FULL) << endl;
    clog << setw(LEFTWIDTH) << "Links m: " << G.edges(Scope::FULL) << endl;
    clog << setw(LEFTWIDTH) << "Self-edges: " << G.selfEdges(Scope::FULL) << endl;
    if(G.isUndirected())
        clog << setw(LEFTWIDTH) << "Undirected edges: " << G.selfEdges(Scope::FULL) + ((G.edges(Scope::FULL) - G.selfEdges(Scope::FULL)) / 2) << endl;
    clog << setw(LEFTWIDTH) << "Average degree: " << G.averageDegree(Scope::FULL) << endl;
    clog << setw(LEFTWIDTH) << "Density: " << G.density(Scope::FULL) << endl;

    // the following metrics are not linear in computation time
    /*
    if(!G.isUndirected())
        clog << setw(LEFTWIDTH) << "Reciprocity: " << G.reciprocity(Scope::FULL) << endl;
    clog << setw(LEFTWIDTH) << "Triangles: " << G.triangles() << endl;
    clog << setw(LEFTWIDTH) << "Wedges: " << G.wedges() << endl;
    clog << setw(LEFTWIDTH) << "Graph clustering coefficient: " << G.graphClusteringCoefficient(Scope::FULL) << endl;
    clog << setw(LEFTWIDTH) << "Average local clustering coefficient: " << G.averageClusteringCoefficient(Scope::FULL) << endl;
    clog << setw(LEFTWIDTH) << "Average distance: " << G.averageDistance(Scope::FULL, 1.0) << endl;
    */

    clog << endl;
    clog << setw(LEFTWIDTH) << "Sorted & unique edge lists: " << (G.isSortedAndUnique() ? "Yes" : "No") << endl;
    clog << setw(LEFTWIDTH) << "Undirected/symmetric edge set: " << (G.isUndirected() ? "Yes" : "No") << endl;
    clog << setw(LEFTWIDTH) << "WCC computed: " << (G.wccComputed() ? "Yes" : "No") << endl;
    clog << setw(LEFTWIDTH) << "SCC computed: " << (G.sccComputed() ? "Yes" : "No") << endl;
    clog << endl;

    if(G.sccComputed()) {
        clog << setw(LEFTWIDTH) << "Strongly connected components: " << G.sccCount() << endl;
        clog << setw(LEFTWIDTH) << "SCC nodes n: " << G.nodes(Scope::LSCC) << endl;
        clog << setw(LEFTWIDTH) << "SCC links m: " << G.edges(Scope::LSCC) << endl;
        clog << setw(LEFTWIDTH) << "SCC self-edges: " << G.selfEdges(Scope::LSCC) << endl;
        clog << setw(LEFTWIDTH) << "SCC average degree: " << G.averageDegree(Scope::LSCC) << endl;
        clog << setw(LEFTWIDTH) << "SCC density: " << G.density(Scope::LSCC) << endl;
        clog << endl;
    }

    if(G.wccComputed()) {
        clog << setw(LEFTWIDTH) << "Weakly connected components: " << G.wccCount() << endl;
        clog << setw(LEFTWIDTH) << "WCC nodes n: " << G.nodes(Scope::LWCC) << endl;
        clog << setw(LEFTWIDTH) << "WCC links m: " << G.edges(Scope::LWCC) << endl;
        clog << setw(LEFTWIDTH) << "WCC self-edges: " << G.selfEdges(Scope::LWCC) << endl;
        if(G.isUndirected())
            clog << setw(LEFTWIDTH) << "WCC undirected edges: " << G.selfEdges(Scope::LWCC) + ((G.edges(Scope::LWCC) - G.selfEdges(Scope::LWCC)) / 2) << endl;
        clog << setw(LEFTWIDTH) << "WCC average degree: " << G.averageDegree(Scope::LWCC) << endl;
        clog << setw(LEFTWIDTH) << "WCC density: " << G.density(Scope::LWCC) << endl;
        clog << endl;
    }
    clog << endl;
} // stats
