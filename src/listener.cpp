/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 * 
 * Listener for standard input/output dialogs with the code
 */

#include "listener.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

// listener function
int listener(Graph & G) {
    clog << "*** Welcome to teexGraph ***" << endl <<
            "- Use standard input (cin) to give commands" << endl <<
            "- Read standard output (cout) to catch the result" << endl <<
            "- Observe standard log (clog) and (cerr) for status and error " <<
            "messages" << endl;

	string command, par;
	vector<long> templong;
	vector<double> tempdouble;
	vector<int> tempint;	
	
	clog << "Input a command: ";
	
	while(cin >> command) {
	
		// loading
		if(command == "load" || command == "load_undirected") {
			clog << "Loading an undirected graph. Enter filename: ";
			cin >> par;
			if(G.loadUndirected(par)) {
				clog << "Loading file succeeded." << endl;
				G.computeWCC();	
			}
			else
				cerr << "Failed loading file." << endl;
		} 
		if(command == "load_directed") {
			clog << "Loading a directed graph. Enter filename: ";
			cin >> par;
			if(G.loadDirected(par)) {
				clog << "Loading file succeeded." << endl;
				G.computeSCC();					
				G.computeWCC();		
			}
			else
				cerr << "Failed loading file." << endl;
				
		} 
		
		// functions allowed when loaded
		else if(G.isLoaded()) {		
		
			Scope scope = FULL;
			// TODO: based on 4 char substring wcc_ or scc_, set scope to LWCC or LSCC
		
			clog << command << endl;	
		
			if(0); 
			// components
			else if(command == "output_wcc") {
				//G.outputWCCNodes();		
			}	
			else if(command == "wcc_distri") {
				//G.printDistri(WCCNodes, false);
			}			
			else if(command == "scc_distri") {
				//G.printDistri(SCCNodes, false);
			}							
		
			// stats
			else if(command == "stats") 
				;//stats();
			else if(command == "nodes") {
				cout << G.nodes(scope) << endl;
			}
			else if(command == "edges") {
				cout << G.edges(scope) << endl;		
			}
			else if(command == "selfedges") {
				cout << G.selfEdges(scope) << endl;		
			}		
			else if(command == "density") {
				cout << G.density(scope) << endl;		
			}		
			else if(command == "reciprocity") {
				cout << G.reciprocity(scope) << endl;		
			}
			else if(command == "averagedegree") {
				cout << G.averageDegree(scope) << endl;		
			}					
			//else if(command == "adj_list") {
				//cin >> par >> filenamepar;
				//getAdjacencyList(atoi(par.c_str()), filenamepar);		
			//}		
			else if(command == "avg_neighbor_degree") {
				//cin >> par >> filenamepar;
				//tempdouble = avgNeighborDegree(atoi(par.c_str()));
				//printPythonNodeList(tempdouble, atoi(par.c_str()), filenamepar);
			}		
			else if(command == "avg_neighbor_local_clustering") {
				//cin >> par >> filenamepar;
				//tempdouble = avgNeighborLocalClustering(atoi(par.c_str()));
				//printPythonNodeList(tempdouble, atoi(par.c_str()), filenamepar);
			}		
				
			// clustering
			else if(command == "local_clustering") {
				//cin >> par >> filenamepar;
				//tempdouble = localClustering(atoi(par.c_str()));
				//printPythonNodeList(tempdouble, atoi(par.c_str()), filenamepar);
			}	
			else if(command == "average_local_clustering" || command == "avg_local_clustering") {
				cout << G.averageClusteringCoefficient(scope) << endl;
			}		
			else if(command == "graph_clustering") {
				cout << G.graphClusteringCoefficient(scope) << endl;
			}				
			else if(command == "triangles") {
				cout << G.triangles() << endl;
			}					
			else if(command == "wedges") {
				cout << G.wedges() << endl;
			}					
		
			// distances
			else if(command == "deg_distri") {
				cin >> par;				
				//printDistri(outDeg, atoi(par.c_str()));
			}			
			else if(command == "dist_distri") {
				cin >> par;				
				//templong = distDistri(atoi(par.c_str()), 1.0);
				//printList(templong, false);
			}	
			else if(command == "est_dist_distri") {
				//cin >> par >> par2;				
				//templong = distDistri(atoi(par.c_str()), atof(par2.c_str()));
				//printList(templong, false);			
			}	
		
			// TODO function for average of distri			
		
			// extreme distances
			else if(command == "eccentricity_distri") {
				//tempint = eccentricitiesBD(); 
				//printDistri(tempint, true);
			}									
			else if(command == "diameter") {
				cout << G.diameterBD() << endl;
			}
			else if(command == "radius") {
				cout << G.radiusBD() << endl;
			}
			else if(command == "center") {
				cout << G.centerSizeBD() << endl;
			}
			else if(command == "periphery") {
				cout << G.peripherySizeBD() << endl;
			}						
				
			// centrality
			else if(command == "degree_centrality" || command == "outdegree1 _centrality") {
				//cin >> par >> filenamepar;				
				//printPythonNodeList(outDeg, atoi(par.c_str()), filenamepar); 
			}
			else if(command == "indegree_centrality") {
				//cin >> par >> filenamepar;				
				//printPythonNodeList(inDeg, atoi(par.c_str()), filenamepar);
			}		
			else if(command == "eccentricity_centrality") {
				//tempint = eccentricitiesBD(); 
				//printPythonNodeList(tempint, true, filenamepar);			
			}
			else if(command == "closeness_centrality")	{
				//cin >> par >> filenamepar;				
				//tempdouble = closenessCentrality(atoi(par.c_str())); 
				//printPythonNodeList(tempdouble, atoi(par.c_str()), filenamepar);
			}
			else if(command == "est_closeness_centrality")	{
				cerr << "Not yet implemented in wrapper." << endl;
			}		
			else if(command == "betweenness_centrality")	{
				//cin >> par >> filenamepar;				
				//tempdouble = betweennessCentrality(atof(par.c_str()), 1.0);
				//printPythonNodeList(tempdouble, atoi(par.c_str()), filenamepar);
			}
			else if(command == "est_betweenness_centrality")	{
				cerr << "Not yet implemented in wrapper." << endl;
			}					

			else {
				cerr << "Command not recognized." << endl;
			}
			cerr << "Input a command: ";		
		} // if loaded
	} // while
	clog << endl << "End of program." << endl;
    return 0;
} // listener
