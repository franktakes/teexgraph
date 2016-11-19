/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 * 
 * BDGraph.h
 */

#ifndef BDGRAPH_H
#define BDGRAPH_H

#include "Graph.h"

class BDGraph : public Graph {
  public:
    BDGraph();
    BDGraph(const int);

    // BD functions
    int centerSizeBD();
    int diameterAPSP();
    int diameterBD();
    vector<int> eccentricitiesAPSP();
    vector<int> eccentricitiesBD();
    int peripherySizeBD();
    int radiusAPSP();
    int radiusBD();

  protected:
    // BD functions:
    int eccentricity(const int);
    int extremaBounding(const int, const bool);
    int pruning();

    // BD data:	
    vector<int> d; // for distance computation
    vector<int> pruned; // -1 if not pruned, 0 or larger value if pruned by that particular node
    vector<int> ecc_lower; // lower eccentricity bounds
    vector<int> ecc_upper; // upper eccentricity bounds
    vector<bool> candidate; // candidate set for contributing to computing the extreme distance measures
};

#endif /* BDGRAPH_H */

