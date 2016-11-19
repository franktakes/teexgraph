/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 * 
 * Examples of how to use the code
 */

#include "examples.h"
#include <iostream>
using namespace std;

// show basic stats of the network
void stats(BDGraph & G) {
    const int LEFTWIDTH = 40;

    // the following metrics are linear in computation time and "free" to show"
    clog << setw(LEFTWIDTH) << "Nodes n: " << G.nodes(FULL) << endl;
    clog << setw(LEFTWIDTH) << "Links m: " << G.edges(FULL) << endl;
    clog << setw(LEFTWIDTH) << "Self-edges: " << G.selfEdges(FULL) << endl;
    if(G.isUndirected())
        clog << setw(LEFTWIDTH) << "Undirected edges: " << G.selfEdges(FULL) + ((G.edges(FULL) - G.selfEdges(FULL)) / 2) << endl;
    clog << setw(LEFTWIDTH) << "Average degree: " << G.averageDegree(FULL) << endl;
    clog << setw(LEFTWIDTH) << "Density: " << G.density(FULL) << endl;

    // the following metrics are not linear in computation time
    /*
    if(!G.isUndirected())
        clog << setw(LEFTWIDTH) << "Reciprocity: " << reciprocity(FULL) << endl;
    clog << setw(LEFTWIDTH) << "Triangles: " << G.triangles() << endl;
    clog << setw(LEFTWIDTH) << "Wedges: " << G.wedges() << endl;
    clog << setw(LEFTWIDTH) << "Graph clustering coefficient: " << G.graphClusteringCoefficient(FULL) << endl;
    clog << setw(LEFTWIDTH) << "Average local clustering coefficient: " << G.averageClusteringCoefficient(FULL) << endl;
    clog << setw(LEFTWIDTH) << "Average distance: " << G.averageDistance(FULL, 1000) << endl;
    */

    clog << endl;
    clog << setw(LEFTWIDTH) << "Sorted & unique edge lists: " << (G.isSortedAndUnique() ? "Yes" : "No") << endl;
    clog << setw(LEFTWIDTH) << "Undirected/symmetric edge set: " << (G.isUndirected() ? "Yes" : "No") << endl;
    clog << setw(LEFTWIDTH) << "WCC computed: " << (G.wccComputed() ? "Yes" : "No") << endl;
    clog << setw(LEFTWIDTH) << "SCC computed: " << (G.sccComputed() ? "Yes" : "No") << endl;
    clog << endl;

    if(G.sccComputed()) {
        clog << setw(LEFTWIDTH) << "Strongly connected components: " << G.sccCount() << endl;
        clog << setw(LEFTWIDTH) << "SCC nodes n: " << G.nodes(LSCC) << endl;
        clog << setw(LEFTWIDTH) << "SCC links m: " << G.edges(LSCC) << endl;
        clog << setw(LEFTWIDTH) << "SCC self-edges: " << G.selfEdges(LSCC) << endl;
        clog << setw(LEFTWIDTH) << "SCC average degree: " << G.averageDegree(LSCC) << endl;
        clog << setw(LEFTWIDTH) << "SCC density: " << G.density(LSCC) << endl;
        clog << endl;
    }

    if(G.wccComputed()) {
        clog << setw(LEFTWIDTH) << "Weakly connected components: " << G.wccCount() << endl;
        clog << setw(LEFTWIDTH) << "WCC nodes n: " << G.nodes(LWCC) << endl;
        clog << setw(LEFTWIDTH) << "WCC links m: " << G.edges(LWCC) << endl;
        clog << setw(LEFTWIDTH) << "WCC self-edges: " << G.selfEdges(LWCC) << endl;
        if(G.isUndirected())
            clog << setw(LEFTWIDTH) << "WCC undirected edges: " << G.selfEdges(LWCC) + ((G.edges(LWCC) - G.selfEdges(LWCC)) / 2) << endl;
        clog << setw(LEFTWIDTH) << "WCC average degree: " << G.averageDegree(LWCC) << endl;
        clog << setw(LEFTWIDTH) << "WCC density: " << G.density(LWCC) << endl;
        clog << endl;
    }
    clog << endl;
} // stats
