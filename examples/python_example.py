from copy import deepcopy

from pyteexgraph import Graph, Scope

g = Graph()
g.loadDirected("/z/edge_list")
print("Loaded", g.isLoaded())
print("Average Degree", g.averageDegree(Scope.FULL))
print("Diameter BD (fails because graph is directed", g.diameterBD())

# Make a deepcopy so we can mutate the graph while preserving the original data
g_undirected = deepcopy(g)
g_undirected.makeUndirected()
g_undirected.computeWCC()

print("Diameter BD", g_undirected.diameterBD())