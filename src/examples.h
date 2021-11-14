/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 *
 * Examples of how to use the code
 */

#pragma once

#include <teexgraph/BDGraph.h>

namespace teexgraph {

void averageDistance(Graph & G, double);
void boundingDiameters(Graph & G);
void clusteringStats(Graph & G, double);
void computeIntensiveStats(Graph & G);
void degreeDistribution(Graph & G);
void distanceDistribution(Graph & G, double);
void stats(Graph & G);
void trianglesWedges(Graph & G);

}
