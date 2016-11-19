# teexGraph

This C++ library is intended for large-scale network analysis and computation.


## About

Several explicit design choices were made in in this library:

* Designed to handle millions of nodes and billions of edges.
* All computation done in-memory and where possible on all available CPU's using OpenMP. 
* Focus is on the analysis of the topology/structure of simple directed and undirected graphs, read from a file. 
* No support for graph models/generators, node/edge attributes, node/edge weights, multi-partite networks, community detection algorithms or visualization (there are other tools that can do that).
* Preference for simplicity over complexity; low-level C++ data structures are used wherever possible, requiring no other libraries than the standard `g++` compiler and the C++ standard library. 


## Features

* Fast computation of the diameter of real-world networks, implementing the *BoundingDiameters* algorithm presented in:

  > F.W. Takes and W.A. Kosters, Determining the Diameter of Small World Networks, in Proceedings of the 20th ACM International Conference on Information and Knowledge Management (CIKM 2011), pp. 1191-1196, 2011. doi: [10.1145/2063576.2063748](http://dx.doi.org/10.1145/2063576.2063748)
  
* Fast computation of extreme distance metrics: the radius, diameter, center size, periphery size and eccentricity distribution of large real-world networks, based on the algorithms presented in:
 
  > F.W. Takes and W.A. Kosters, Computing the Eccentricity Distribution of Large Graphs, Algorithms 6(1): 100-118, 2013. doi: [10.3390/a6010100](http://dx.doi.org/10.3390/a6010100)

* Computation of closeness centrality in parallel, exact or approximated using the method discussed in:

  > D. Eppstein and J. Wang, Fast Approximation of Centrality, Journal of Graph Algorithms and Applications 8(1): 39--45, 2004. doi: [10.7155/jgaa.00081](http://dx.doi.org/10.7155/jgaa.00081)

* Computation of betweenness centrality in parallel, exact or an adapted approximate version of the method introduced in:

  > U. Brandes, A Faster Algorithm for Betweenness Centrality, Journal of Mathematical Sociology 25(2): 163-177, 2001.

Of course, all credit goes to the original authors of these algorithms. 


## Usage and Installation

* Install using `make` and then run the executable `teexgraph`. 
* Consult the [main.cpp](src/main.cpp) and [examples.hpp](src/examples.hpp) files for examples of how to use the code. 
* The library supports networks with up to INT_MAX (2 billion) nodes and as many edges as the memory of your machine can hold. If you want to have more than 10 million nodes or just want to optimize memory usage, pass some integer equal to or greater than the expected node count as one integer parameter to the constructor. 
* The program expects a whitespace-separated edge list (indicated by node pairs) as input format. By default, it assumes nodes of the type `long`. This can be changed to for example `string` in [Graph.h](src/main.cpp) at the cost of a factor 2 or more in loading speed. Friendly input files are expected; although comments (lines that do not start with a character in [a-zA-Z0-9]) at the beginning of the input file are allowed. After the first line with an edge, only edges are expected.


## Disclaimer

This code was written for research-purposes only, and is and should in no way be seen as an attempt at creating a good piece of code with respect to any programming- or software-engineering standards whatsoever. 
It comes without any warranty of merchantability or suitability for a particular purpose. 
The software has exclusively been tested under the UNIX/Linux operating system, in particular Ubuntu LTS (12.04, 14.04 and 16.04) and CentOS (6.7 and 7). 
