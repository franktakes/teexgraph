#include <teexgraph/Graph.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cstdint>
#include <memory>

using namespace teexgraph;

namespace py = pybind11;

std::unique_ptr<Graph> make_graph_from_vectors(
    const std::vector<int64_t>& sources,
    const std::vector<int64_t>& targets
){
    return std::make_unique<Graph>(sources, targets);
}



PYBIND11_MODULE(pyteexgraph, m) {
  m.doc() = "teexGraph for Python";

  py::enum_<Scope>(m, "Scope")
      .value("FULL", Scope::FULL)
      .value("LWCC", Scope::LWCC)
      .value("LSCC", Scope::LSCC)
      .export_values();

  py::class_<Graph>(m, "Graph")
    .def("__deepcopy__", [](const Graph &self, py::dict) {
        return Graph(self);
    }, "memo")

    .def(py::init<>())
    .def(py::init<const int>())
    .def(py::init<const std::string&, const bool>(), py::arg("filename"), py::arg("directed"))
    .def(py::init(&make_graph_from_vectors), py::arg("sources"), py::arg("targets"))

    .def("loadDirected", &Graph::loadDirected, py::arg("filename"))
    .def("loadUndirected", &Graph::loadUndirected, py::arg("filename"))
    .def("loadDirectedFromVectors",
        [](
            Graph &self,
            const std::vector<int64_t>& sources,
            const std::vector<int64_t>& targets
        ){ self.loadDirectedFromVectors(sources, targets); },
        py::arg("sources"), py::arg("targets")
    )

    .def("clear", &Graph::clear)

    // status
    .def("isLoaded", &Graph::isLoaded)
    .def("isUndirected", &Graph::isUndirected)
    .def("isSortedAndUnique", &Graph::isSortedAndUnique)
    .def("sccComputed", &Graph::sccComputed)
    .def("wccComputed", &Graph::wccComputed)
    .def("inScope", &Graph::inScope, py::arg("node_id"), py::arg("scope"))

    // manipulation
    .def("makeUndirected", &Graph::makeUndirected)

    // components
    .def("computeSCC", &Graph::computeSCC)
    .def("computeWCC", &Graph::computeWCC)

    // basic topology
    .def("averageDegree", &Graph::averageDegree, py::arg("scope"))
    .def("density", &Graph::density, py::arg("scope"))
    .def("edges", &Graph::edges, py::arg("scope"))
    .def("nodes", &Graph::nodes, py::arg("scope"))
    // .def("nodesInScc", &Graph::nodesInScc, py::arg("scc_id")) //TODO
    .def("nodesInWcc", &Graph::nodesInWcc, py::arg("wcc_id"))
    .def("reciprocity", &Graph::reciprocity, py::arg("scope"))
    .def("sccCount", &Graph::sccCount)
    .def("selfEdges", &Graph::selfEdges, py::arg("scope"))
    .def("wccCount", &Graph::wccCount)
    .def("wccOf", &Graph::wccOf, py::arg("wcc_id"))
    .def("revNeighbors", &Graph::revNeighbors, py::arg("node_id"))
    .def("neighbors", &Graph::neighbors, py::arg("node_id"))

    // BoundingDiameters functions
    .def("centerSizeBD", &Graph::centerSizeBD)
    .def("diameterAPSP", &Graph::diameterAPSP)
    .def("diameterBD", &Graph::diameterBD)
    .def("peripherySizeBD", &Graph::peripherySizeBD)
    .def("radiusAPSP", &Graph::radiusAPSP)
    .def("radiusBD", &Graph::radiusBD)
    .def("eccentricitiesAPSP", &Graph::eccentricitiesAPSP)
    .def("eccentricitiesBD", &Graph::eccentricitiesBD)

    // triangles and clustering
    .def("averageClusteringCoefficient", &Graph::averageClusteringCoefficient, py::arg("scope"), py::arg("sample_fraction")) 
    .def("graphClusteringCoefficient", &Graph::graphClusteringCoefficient, py::arg("scope"))
    .def("triangles", &Graph::triangles, py::arg("scope"))
    .def("wedges", &Graph::wedges, py::arg("scope"))
    .def("nodeClusteringCoefficient", &Graph::nodeClusteringCoefficient, py::arg("node_id"))
    //.def("localClustering", &Graph::localClustering, py::arg("scope")) // temporarily disabled due to new argument type
    // pair<long, long> trianglesWedgesAround(const int);

	// centrality
    .def("betweennessCentrality", &Graph::betweennessCentrality, py::arg("scope"), py::arg("sample_fraction"))
    .def("closenessCentrality", &Graph::closenessCentrality, py::arg("scope"), py::arg("sample_fraction"))
    .def("degreeCentrality", &Graph::degreeCentrality)
    .def("eccentricityCentrality", &Graph::eccentricityCentrality, py::arg("scope"))
    .def("indegreeCentrality", &Graph::indegreeCentrality)
    .def("outdegreeCentrality", &Graph::outdegreeCentrality)
    .def("pageRankCentrality", &Graph::pageRankCentrality)

    // distances, degree and compont size distributions
    .def("distanceDistribution", &Graph::distanceDistribution, py::arg("scope"), py::arg("sample_fraction"))
    .def("indegreeDistribution", &Graph::indegreeDistribution, py::arg("scope"))
    .def("outdegreeDistribution", &Graph::outdegreeDistribution, py::arg("scope"))
    .def("sccSizeDistribution", &Graph::sccSizeDistribution)
    .def("wccSizeDistribution", &Graph::wccSizeDistribution)

    // distance metrics
    .def("distance", &Graph::distance, py::arg("node1_id"), py::arg("node2_id"))
    // .def("distances", &Graph::distances, (const int, vector<long> &);
	.def("alldistances", &Graph::alldistances, py::arg("node_id"))
    .def("averageDistance", &Graph::averageDistance, py::arg("scope"), py::arg("sample_fraction"));

/*
    // (node)list, distribution and stdout/binary output formats
#include "GraphTemplated.hpp"

*/
}
