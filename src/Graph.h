/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 * 
 * Graph class header
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm> // min, max, etc.
#include <cassert> // min, max, etc.
#include <climits> // INT_MAX etc.
#include <cstdlib>
#include <fstream> // file reading/writing
#include <iomanip> // setw()
#include <iostream> // cout, clog, cerr, etc.
#include <map> // mapping node id's
#include <string> // filenames etc.
#include "omp.h" // openMP paralellization
#include <queue> // BFS
#include <stack> // DFS
#include <vector> // node data structure
using namespace std;

// node datatype in input file. in terms of speed, int > long > string
typedef long nodeidtype;

// diferent scopes at which we can call functions: on the FULL network, on the
// largest weakly (WCC) or strongly (SCC) connected component 
enum Scope {

    FULL, WCC, SCC
};
class Graph {

public:
    // initialization and loading
    Graph();
    void clear();
    bool loadDirected(const string);
    bool loadUndirected(const string);

    // components        
    void computeSCC();
    void computeWCC();

    // (node)list, distribution and stdout/binary output formats
#include "GraphTemplated.hpp"

    // basic topology
    double averageDegree(const Scope);
    double density(const Scope);
    int nodes(const Scope);
    int edges(const Scope);
    double reciprocity(const Scope scope);
    int selfEdges(const Scope scope);
    void stats();

    // triangles and clustering
    double averageClusteringCoefficient(const Scope);
    double graphClusteringCoefficient(const Scope);
    vector<double> localClustering(const Scope);
    double nodeClusteringCoefficient(const int);
    long triangles();
    pair<long, long> trianglesWedgesAround(const int);
    long wedges();

    // distances, degree and compont size distributions
    vector<long> distanceDistribution(const Scope, const double);
    void indegreeDistribution(const Scope);
    void outdegreeDistribution(const Scope);
    void sccSizeDistribution();
    void wccSizeDistribution();

    // distance metrics
    int distance(const int, const int);
    vector<int> distances(const int, vector<long> &);

    // external querying
    vector<int> & neighbors(const int);

    vector< vector<int> > E; // list of out-neighbors of i 		
    bool isUndirected();
    
protected:

    bool addEdge(const int, const int);
    bool edge(const int, const int);
    bool edgeSlow(const int, const int);
    void goMarkSCC(const int, const int);
    void goVisitSCC(const int, vector<bool> &, stack<int> &, vector<int> &, vector<int> &);
    bool inScope(const int, const Scope);
    void makeUndirected();    
    int mapNode(const nodeidtype);
    nodeidtype revMapNode(const int);
    void sortEdgeList();

//private:

    // graph data, always consistent
    static const int MAXN = 10000000; // maximal number of nodes
    map<nodeidtype, int> nodeMapping; // mapping of input node-identifiers to 0, .., n-1
    map<int, nodeidtype> revMapping; // mapping 0, .., n-1 to input node-identifiers 
    vector< vector<int> > rE; // list of in-neighbors of i	
    vector<long> inDeg; // indegrees
    vector<long> outDeg; // outdegrees
    int n; // number of nodes										
    long m; // number of links
    vector<bool> hasSelfLoop; // true if node at index has a self-loop
    long selfm; // number of self-loops (self-edges) 
    int nexti; // next unused node id (finally equal to n)

    // graph type status
    bool loaded; // is the graph already loaded?
    bool sortedandunique; // are the edge lists sorted and with unique values?
    bool undirected; // does the graph have a symmetric edge set?
    bool doneSCC; // has the SCC for each node been computed?
    bool doneWCC; // has the WCC for each node been computed?

    // Connected components
    int largestWCC; // index of largest WCC 
    int largestSCC; // index of largest SCC 
    int wccs; // number of WCCs
    int sccs; // number of WCCs
    vector<int> wccId; // WCC # of node i
    vector<int> wccNodes; // nr. of nodes in WCC i								
    vector<long> wccEdges; // nr. of edges in WCC i				
    vector<int> sccId; // SCC # of node i
    vector<int> sccNodes; // nr. of nodes in SCC i								
    vector<long> sccEdges; // nr. of edges in SCC i		
};

#endif /* GRAPH_H */

