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
#include <string> // filenames etc.
#include "omp.h" // openMP paralellization
#include <queue> // BFS
#include <stack> // DFS
#include <unordered_map> // mapping node id's
#include <vector> // node data structure

namespace teexgraph {

// node datatype in input file. for optimal loading speed, use long or int (not string)
typedef long nodeidtype;

// default max. node count; change here or by passing int to Graph constructor
const int MAXN = 10000000;

// diferent scopes at which we can call functions: on the FULL network, on the
// largest weakly (WCC) or strongly (SCC) connected component

enum class Scope {
    FULL, LWCC, LSCC
};

enum class Extremum {
    DIAMETER,
    RADIUS,
    ECCENTRICITIES,
    PERIPHERY_SIZE,
    CENTER_SIZE
};

class Graph {
  public:
    // initialization and loading
    Graph();
    Graph(const int);
    void clear();
    bool loadDirected(const std::string);
    bool loadUndirected(const std::string);
	bool inScope(const int, const Scope);

    // components
    void computeSCC();
    void computeWCC();

    // (node)list, distribution and stdout/binary output formats
#include <teexgraph/details/GraphTemplated.hpp>

    // basic topology
    double averageDegree(const Scope);
    double density(const Scope);
    long edges(const Scope);
    std::vector<int> & neighbors(const int);
    int nodes(const Scope);
    int nodesInScc(const int);
    int nodesInWcc(const int);
    double reciprocity(const Scope scope);
    std::vector<int> & revNeighbors(const int);
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
    double approximateAverageClusteringCoefficient(const Scope, const double);    
    double graphClusteringCoefficient(const Scope);
    std::vector<double> approximateLocalClustering(std::vector<int> &);    
    std::vector<double> localClustering(const Scope);
    double nodeClusteringCoefficient(const int);
    long triangles(const Scope);
    std::pair<long, long> trianglesWedgesAround(const int);
    long wedges(const Scope);

    // distances, degree and compont size distributions
    std::vector<long> distanceDistribution(const Scope, const double);
    std::vector<long> indegreeDistribution(const Scope);
    std::vector<long> outdegreeDistribution(const Scope);
    std::vector<int> sccSizeDistribution();
    std::vector<int> wccSizeDistribution();

    // distance metrics
    int distance(const int, const int);
    std::vector<int> distances(const int, std::vector<long> &);
	std::vector<int> alldistances(const int);
    double averageDistance(const Scope, const double);

    // BoundingDiameters functions
    int centerSizeBD();
    int diameterAPSP();
    int diameterBD();
    std::vector<int> eccentricitiesAPSP();
    std::vector<int> eccentricitiesBD();
    int peripherySizeBD();
    int radiusAPSP();
    int radiusBD();

	// centrality
    std::vector<double> betweennessCentrality(const Scope, const double);
    std::vector<double> closenessCentrality(const Scope, const double);
    std::vector<double> degreeCentrality();
    std::vector<double> eccentricityCentrality(const Scope);
    std::vector<double> indegreeCentrality();
    std::vector<double> outdegreeCentrality();
    std::vector<double> pageRankCentrality();

    // listener stuff
    void writeBinaryAdjacencyList(const Scope, std::string);

    void makeUndirected();

  protected:

    bool addEdge(const int, const int);
    bool edge(const int, const int);
    bool edgeSlow(const int, const int);
    void goMarkSCC(const int, const int);
    void goVisitSCC(const int, std::vector<bool> &, std::stack<int> &, std::vector<int> &, std::vector<int> &);
    int mapNode(const nodeidtype);
    nodeidtype revMapNode(const int);
    void sortEdgeList();

  private:

    // graph data, always consistent
    int maxn; // maximal number of nodes
    std::unordered_map<nodeidtype, int> nodeMapping; // mapping of input node-identifiers to 0, .., n-1
    std::unordered_map<int, nodeidtype> revMapping; // mapping 0, .., n-1 to input node-identifiers
    std::vector< std::vector<int> > E; // list of out-neighbors of i
    std::vector< std::vector<int> > rE; // list of in-neighbors of i

    int n; // number of nodes
    long m; // number of links
    std::vector<bool> hasSelfLoop; // true if node at index has a self-loop
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
    std::vector<int> wccId; // WCC # of node i
    std::vector<int> wccNodes; // nr. of nodes in WCC i
    std::vector<long> wccEdges; // nr. of edges in WCC i
    std::vector<int> sccId; // SCC # of node i
    std::vector<int> sccNodes; // nr. of nodes in SCC i
    std::vector<long> sccEdges; // nr. of edges in SCC i

	// helpers for sampling
	double setSampleSize(int &, const Scope, const double);
	std::vector<int> getSample(const int, const Scope);

    // BoundingDiameters functions:
    int eccentricity(const int);
    int extremaBounding(Extremum, const bool);
    int pruning();

    // BoundingDiameters data:
    std::vector<int> d; // for distance computation
    std::vector<int> pruned; // -1 if not pruned, 0 or larger value if pruned by that particular node
    std::vector<int> ecc_lower; // lower eccentricity bounds
    std::vector<int> ecc_upper; // upper eccentricity bounds
    std::vector<bool> candidate; // candidate set for contributing to computing the extreme distance measures

	// centrality
    double closeness(const int);
    std::vector<int> closenesses(const int, std::vector<long> &);
    int closenessSum(const int);
};

}

#endif /* GRAPH_H */
