# teexGraph

This C++ library is intended for large-scale network analysis and computation.


## About

Several explicit design choices were made in in this library:

* Designed to handle millions of nodes and billions of edges.
* All computation done in-memory and where possible on all available CPU's using OpenMP.
* Focus is on the analysis of the topology/structure of simple directed and undirected graphs, read from a file.
* Particular focus on distance-related functions for computing the exact and approximate distance distribution, distance-based network metrics such as the diameter, as well as centrality measures such as closeness and betweenness centrality.
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


## Compilation

If you want the Python library you'll need [PyBind11](https://github.com/pybind/pybind11). You can install it with:
```bash
sudo apt install pybind11-dev
```

Compile with the standard cmake sequence:
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
make
```
or make a debugging build with
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

## Usage

* The main way of using this code is by running the required code directly from C++. Consult the [main.cpp](src/main.cpp) and [examples.cpp](src/examples.cpp) files for examples of how to use the code. Alternatively, there is funtionality to use the code directly using Python package pyteexgraph. 
* The library supports networks with up to INT_MAX (2 billion) nodes and as many edges as the memory of your machine can hold. If you want to have more than 10 million nodes or just want to optimize memory usage, pass some integer equal to or greater than the expected node count as one integer parameter to the constructor.
* The program expects a whitespace-separated edge list (indicated by node pairs) as input format. By default, it assumes nodes of the type `long`. This can be changed to for example `string` in [Graph.h](src/main.cpp) at the cost of a factor 2 or more in loading speed. Friendly input files are expected; although comments (lines that do not start with a character in [a-zA-Z0-9]) at the beginning of the input file are allowed. After the first line with an edge, only edges are expected.


## Use as a Python library

If PyBind11 is installed and teexGraph is compiled as above a file with a name ismilar to
```text
build/pyteexgraph.cpython-39-x86_64-linux-gnu.so
```
will be generated. Make sure this is accessible from your Python working directory  and use 
```
import pyteexgraph
```
from within Python to load it.


## Use as a C++ library

If your project uses cmake, then incorporating kilosim is easy!

Just add the following lines to your `CMakeLists.txt` file:
```text
add_subdirectory(path/to/teexgraph EXCLUDE_FROM_ALL)
target_link_libraries(your_executable_target PRIVATE teexgraphlib)
```

And everything should just work.


## Disclaimer

This code was written for research-purposes only, and is and should in no way be seen as an attempt at creating a good piece of code with respect to any programming- or software-engineering standards whatsoever.
It comes without any warranty of merchantability or suitability for a particular purpose.
The software has exclusively been tested under the UNIX/Linux operating system, in particular Ubuntu LTS (12.04, 14.04 and 16.04) and CentOS (6.7 and 7).
