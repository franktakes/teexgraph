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
#include <omp.h> // openMP paralellization
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
    Graph(const std::string& filename, const bool directed);
    template<
        typename Integer, 
        typename = typename std::enable_if<std::is_integral<Integer>::value, Integer>::type
    >
    Graph(
        const std::vector<Integer>& sources,
        const std::vector<Integer>& targets
    ){
        loadDirectedFromVectors(sources, targets);
    }

    void clear();

    // Loaders
    bool loadDirected(const std::string);
    bool loadUndirected(const std::string);
    template<
        typename Integer//,
//        typename = typename std::enable_if<std::is_integral<Integer>::value, Integer>::type
    >
    void loadDirectedFromVectors(
        const std::vector<Integer>& sources,
        const std::vector<Integer>& targets
    );

    void makeUndirected();
    bool inScope(const int, const Scope) const;

    // components
    void computeSCC();
    void computeWCC();

    // (node)list, distribution and stdout/binary output formats
#include <teexgraph/details/GraphTemplated.hpp>

    // basic topology
    double averageDegree(const Scope) const;
    double density(const Scope) const;
    bool edge(const int, const int);
    long edges(const Scope) const;
    int mapNode(const nodeidtype);
    const std::vector<int> & neighbors(const int) const;
    int nodes(const Scope) const;
    int nodesInScc(const int) const;
    int nodesInWcc(const int) const;
    double reciprocity(const Scope scope);
    nodeidtype revMapNode(const int) const;
    const std::vector<int> & revNeighbors(const int) const;
    int sccCount() const;
    long selfEdges(const Scope scope) const;
    int wccCount() const;
    int wccOf(const int) const;

    // status
    bool isLoaded() const;
    bool isUndirected() const;
    bool isSortedAndUnique() const;
    bool sccComputed() const;
    bool wccComputed() const;

    // triangles and clustering
    double averageClusteringCoefficient(const Scope, const double);
    double graphClusteringCoefficient(const Scope);
    std::vector<double> localClustering(std::vector<int> &);
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
    int distance(const int, const int) const;
    std::vector<int> distances(const int, std::vector<long> &) const;
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

  protected:

    bool addEdge(const int, const int);
    bool edgeSlow(const int, const int);
    void goMarkSCC(const int, const int);
    void goVisitSCC(const int, std::vector<bool> &, std::stack<int> &, std::vector<int> &, std::vector<int> &);
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
    bool loaded = false; // is the graph already loaded?
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




template<
    typename Integer
//    typename = typename std::enable_if<std::is_integral<Integer>::value, Integer>::type
>
void Graph::loadDirectedFromVectors(
    const std::vector<Integer>& sources,
    const std::vector<Integer>& targets
){
    if(sources.size()!=targets.size()){
        throw std::runtime_error("Sources and Targets must be of the same length!");
    }
    long edgesAdded = 0;
    long edgesSkipped = 0;

    // check if not already loaded
    if(loaded) {
        throw std::runtime_error("Graph already loaded!");
    }

    maxn = sources.size();
    clear();

    // load the edge list
    for(size_t i=0;i<sources.size();i++){
        if(addEdge(mapNode(sources[i]), mapNode(targets[i]))){
            edgesAdded++;
        } else {
            edgesSkipped++;
        }
    }

    loaded = true;

    if(edgesSkipped == 0) {
        sortEdgeList();
    } else {
        throw std::runtime_error("Out-of-bounds: increase maxn in Graph.h!");
    }
}



}

#endif /* GRAPH_H */
