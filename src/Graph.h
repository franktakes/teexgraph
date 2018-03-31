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

// node datatype in input file. for optimal loading speed, use long or int (not string)
typedef long nodeidtype;

// default max. node count; change here or by passing int to Graph constructor
const int MAXN = 10000000;

// diferent scopes at which we can call functions: on the FULL network, on the
// largest weakly (WCC) or strongly (SCC) connected component 

enum Scope {
    FULL, LWCC, LSCC
};

class Graph {
  public:
    // initialization and loading
    Graph();
    Graph(const int);
    void clear();
    bool loadDirected(const string);
    bool loadUndirected(const string);
	bool inScope(const int, const Scope);

    // components        
    void computeSCC();
    void computeWCC();

    // (node)list, distribution and stdout/binary output formats
#include "GraphTemplated.hpp"

    // basic topology
    double averageDegree(const Scope);
    double density(const Scope);
    long edges(const Scope);
    vector<int> & neighbors(const int);
    int nodes(const Scope);
    int nodesInScc(const int);    
    int nodesInWcc(const int);
    double reciprocity(const Scope scope);
    vector<int> & revNeighbors(const int);
    int sccCount();
    long selfEdges(const Scope scope);
    int wccCount();
    int wccOf(const int);
    
    // status
    bool isLoaded();    
    bool isUndirected();
    bool isSortedAndUnique();    
    bool sccComputed();
    bool wccComputed();

    // triangles and clustering
    double averageClusteringCoefficient(const Scope);
    double graphClusteringCoefficient(const Scope);
    vector<double> localClustering(const Scope);
    double nodeClusteringCoefficient(const int);
    long triangles(const Scope);
    pair<long, long> trianglesWedgesAround(const int);
    long wedges(const Scope);

    // distances, degree and compont size distributions
    vector<long> distanceDistribution(const Scope, const double);
    vector<long> indegreeDistribution(const Scope);
    vector<long> outdegreeDistribution(const Scope);
    vector<int> sccSizeDistribution();
    vector<int> wccSizeDistribution();

    // distance metrics
    int distance(const int, const int);
    vector<int> distances(const int, vector<long> &);
	vector<int> alldistances(const int);
    double averageDistance(const Scope, const double);
    
    // BoundingDiameters functions
    int centerSizeBD();
    int diameterAPSP();
    int diameterBD();
    vector<int> eccentricitiesAPSP();
    vector<int> eccentricitiesBD();
    int peripherySizeBD();
    int radiusAPSP();
    int radiusBD();
    
	// centrality
    vector<double> betweennessCentrality(const Scope, const double);
    vector<double> closenessCentrality(const Scope, const double);
    vector<double> degreeCentrality();
    vector<double> eccentricityCentrality(const Scope);
    vector<double> indegreeCentrality();
    vector<double> outdegreeCentrality();
    vector<double> pageRankCentrality();
	
    // listener stuff
    void writeBinaryAdjacencyList(const Scope, string);

  protected:

    bool addEdge(const int, const int);
    bool edge(const int, const int);
    bool edgeSlow(const int, const int);
    void goMarkSCC(const int, const int);
    void goVisitSCC(const int, vector<bool> &, stack<int> &, vector<int> &, vector<int> &);
    void makeUndirected();
    int mapNode(const nodeidtype);
    nodeidtype revMapNode(const int);
    void sortEdgeList();

  private:

    // graph data, always consistent
    int maxn; // maximal number of nodes
    map<nodeidtype, int> nodeMapping; // mapping of input node-identifiers to 0, .., n-1
    map<int, nodeidtype> revMapping; // mapping 0, .., n-1 to input node-identifiers 
    vector< vector<int> > E; // list of out-neighbors of i 		
    vector< vector<int> > rE; // list of in-neighbors of i	

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
    
    // BoundingDiameters functions:
    int eccentricity(const int);
    int extremaBounding(const int, const bool);
    int pruning();

    // BoundingDiameters data:	
    vector<int> d; // for distance computation
    vector<int> pruned; // -1 if not pruned, 0 or larger value if pruned by that particular node
    vector<int> ecc_lower; // lower eccentricity bounds
    vector<int> ecc_upper; // upper eccentricity bounds
    vector<bool> candidate; // candidate set for contributing to computing the extreme distance measures
  
	// centrality
    double closeness(const int);
    vector<int> closenesses(const int, vector<long> &);
    int closenessSum(const int);
	  
};

#endif /* GRAPH_H */

