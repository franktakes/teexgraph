/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 *
 * Examples of how to use the code
 */

#include "examples.h"

#include <iostream>

using namespace std;

namespace teexgraph {

// output how often each distance value occurs
void distanceDistribution(Graph & G, double samplingRate = 1.0) {
	vector<long> distances;
	// second parameter "1.0" can be used to state a sampling percentage
    if(G.wccComputed())
    	distances = G.distanceDistribution(Scope::LWCC, samplingRate);
    else
    	distances = G.distanceDistribution(Scope::FULL, samplingRate);
    G.printList(distances);
}

// show basic stats of the network
void stats(Graph & G) {
    const int LEFTWIDTH = 40;

    // the following metrics are linear in computation time and "free" to show"
    cout << setw(LEFTWIDTH) << "Nodes n: " << G.nodes(Scope::FULL) << endl;
    cout << setw(LEFTWIDTH) << "Links m: " << G.edges(Scope::FULL) << endl;
    cout << setw(LEFTWIDTH) << "Self-edges: " << G.selfEdges(Scope::FULL) << endl;
    if(G.isUndirected())
        cout << setw(LEFTWIDTH) << "Undirected edges: " << G.selfEdges(Scope::FULL) + ((G.edges(Scope::FULL) - G.selfEdges(Scope::FULL)) / 2) << endl;
    cout << setw(LEFTWIDTH) << "Average degree: " << G.averageDegree(Scope::FULL) << endl;
    cout << setw(LEFTWIDTH) << "Density: " << G.density(Scope::FULL) << endl;

    // the following metrics are not linear in computation time
    
    if(!G.isUndirected())
        cout << setw(LEFTWIDTH) << "Reciprocity: " << G.reciprocity(Scope::FULL) << endl;

    cout << endl;
    cout << setw(LEFTWIDTH) << "Sorted & unique edge lists: " << (G.isSortedAndUnique() ? "Yes" : "No") << endl;
    cout << setw(LEFTWIDTH) << "Undirected/symmetric edge set: " << (G.isUndirected() ? "Yes" : "No") << endl;
    cout << setw(LEFTWIDTH) << "WCC computed: " << (G.wccComputed() ? "Yes" : "No") << endl;
    cout << setw(LEFTWIDTH) << "SCC computed: " << (G.sccComputed() ? "Yes" : "No") << endl;
    cout << endl;

    if(G.sccComputed()) {
        cout << setw(LEFTWIDTH) << "Strongly connected components: " << G.sccCount() << endl;
        cout << setw(LEFTWIDTH) << "SCC nodes n: " << G.nodes(Scope::LSCC) << endl;
        cout << setw(LEFTWIDTH) << "SCC links m: " << G.edges(Scope::LSCC) << endl;
        cout << setw(LEFTWIDTH) << "SCC self-edges: " << G.selfEdges(Scope::LSCC) << endl;
        cout << setw(LEFTWIDTH) << "SCC average degree: " << G.averageDegree(Scope::LSCC) << endl;
        cout << setw(LEFTWIDTH) << "SCC density: " << G.density(Scope::LSCC) << endl;
        cout << endl;
    }

    if(G.wccComputed()) {
        cout << setw(LEFTWIDTH) << "Weakly connected components: " << G.wccCount() << endl;
        cout << setw(LEFTWIDTH) << "WCC nodes n: " << G.nodes(Scope::LWCC) << endl;
        cout << setw(LEFTWIDTH) << "WCC links m: " << G.edges(Scope::LWCC) << endl;
        cout << setw(LEFTWIDTH) << "WCC self-edges: " << G.selfEdges(Scope::LWCC) << endl;
        if(G.isUndirected())
            cout << setw(LEFTWIDTH) << "WCC undirected edges: " << G.selfEdges(Scope::LWCC) + ((G.edges(Scope::LWCC) - G.selfEdges(Scope::LWCC)) / 2) << endl;
        cout << setw(LEFTWIDTH) << "WCC average degree: " << G.averageDegree(Scope::LWCC) << endl;
        cout << setw(LEFTWIDTH) << "WCC density: " << G.density(Scope::LWCC) << endl;
        cout << endl;
    }
    //cout << endl;
} // stats

void computeIntensiveStats(Graph & G) {
    const int LEFTWIDTH = 40;
    
    // exact computation, in parallel
    cout << setw(LEFTWIDTH) << "Triangles: " << G.triangles(Scope::FULL) << endl;
    cout << setw(LEFTWIDTH) << "Wedges: " << G.wedges(Scope::FULL) << endl;
    
    // approximated, in parallel (also output with distanceDistribution() function below)
    //cout << setw(LEFTWIDTH) << "Average distance: " << G.averageDistance(Scope::FULL, 200) << endl;
}

void degreeDistribution(Graph & G) {
    vector<long> degs = G.outdegreeDistribution(Scope::LWCC);
    G.printList(degs);
}

void clusteringStats(Graph & G) {
    const int LEFTWIDTH = 40;
    //cout << setw(LEFTWIDTH) << "Graph clustering coefficient: " << G.graphClusteringCoefficient(Scope::FULL) << endl;    
	//cout << setw(LEFTWIDTH) << "Average local clustering coefficient: " << G.averageClusteringCoefficient(Scope::FULL) << endl;
	cout << setw(LEFTWIDTH) << "Approx. avg. loc. clust. coefficient: " << G.approximateAverageClusteringCoefficient(Scope::FULL, 5) << endl;
}

void boundingDiameters(Graph & G) {
    const int LEFTWIDTH = 40;
	
	int diameter = G.diameterBD();
    cout << setw(LEFTWIDTH) << "WCC diameter: " << diameter << endl;

	//int radius = G.radiusBD();
    //cout << setw(LEFTWIDTH) << "WCC radius: " << radius << endl;

	//int periphery = G.peripherySizeBD();
    //cout << setw(LEFTWIDTH) << "WCC periphery size: " << periphery << endl;
	
	//int center = G.centerSizeBD();
    //cout << setw(LEFTWIDTH) << "WCC center size: " << center << endl;

	// eccentricity distribution
    //vector<int> eccs = G.eccentricitiesBD();
	//G.printDistri(eccs, Scope::LWCC);
}

}
