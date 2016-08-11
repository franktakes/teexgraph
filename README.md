# tyxGraph

This C++ library is intended for the analysis of large-scale network data. 

## About

Several explicit design choices were made in tyxGraph:

* Designed to handle millions of nodes and billions of edges.
* All computation done in-memory and where possible on all available CPU's using OpenMP. 
* Focus is on the analysis of the topology/structure of simple directed and undirected graphs, read from a file. 
* No support for graph models/generators, node/edge attributes, node/edge weights, multi-partite networks or visualization (there are other tools that can do that).
* Preference for simplicity over complexity; low-level C++ data structures are used wherever possible, requiring no other libraries than the standard g++ compiler and the C++ standard library. 

## Usage and Installation

* Install using "make" and then run the executable "tyxgraph". 
* The library supports networks with up to INT_MAX (2 billion) nodes and as many edges as the memory of your machine can hold.
* Friendly input files are expected; although comments (lines that do not start with a character in [0-9]) at the beginning of an edge file are allowed.
    After the first line with a pair of integers representing an edge, only pairs of integers are expected.
* Consult the main.cpp file for examples of how to use the code. 

## Disclaimer

This code was written for research-purposes only, and is and should in no way be seen as an attempt at creating a good piece of code with respect to any programming- or software-engineering standards whatsoever. 
It comes without any warranty of merchantability or suitability for a particular purpose. 
I do hope it helps you in your research. 
The software has exclusively been tested under the UNIX operating system.
