/*
 * Example main-file showing how to use tyxgraph
 */

/* 
 * File:   main.cpp
 * Author: ftakes1
 *
 * Created on August 8, 2016, 12:09 PM
 */

#include <cstdlib>
#include "Graph.h"
using namespace std;

int main(int argc, char** argv) {
	static Graph G;
	G.loadUndirected(argv[1]); // TODO ca-AstroPh goes wrong complete with WCC node and edge count
	G.calcSCC();
	G.calcWCC();
	G.stats();
	G.wccDistri();
	return 0;
}

