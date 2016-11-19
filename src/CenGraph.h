/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 * 
 * CenGraph.h
 */


#ifndef CENGRAPH_H
#define CENGRAPH_H

#include "BDGraph.h"

class CenGraph : public BDGraph {
  public:
    CenGraph();
    CenGraph(const int);
    vector<double> betweennessCentrality(const Scope, const double);
    vector<double> closenessCentrality(const Scope, const double);
    vector<double> degreeCentrality();
    vector<double> eccentricityCentrality(const Scope);
    vector<double> indegreeCentrality();
    vector<double> outdegreeCentrality();
    vector<double> pageRankCentrality();

  protected:
    double closeness(const int);
    vector<int> closenesses(const int, vector<long> &);
    int closenessSum(const int);

};

#endif /* CENGRAPH_H */

