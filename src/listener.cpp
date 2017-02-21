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
            "messages" << endl << 
            "- Graphs up to MAXN = " << MAXN << " nodes are accepted" << endl;

	string command, par, par2;
	Scope scope; 
	vector<long> templong;
	vector<double> tempdouble;
	vector<int> tempint;	
	
	clog << "Input a command: ";
	
	while(cin >> command) {
	
		transform(command.begin(), command.end(), command.begin(), ::tolower);
	
		templong.empty();
		tempdouble.empty();
		tempint.empty();
	
		////////////////////////////////////////// functions allowed when loaded
		if(G.isLoaded()) {		
		
			// determine scope
			scope = FULL;
			if(command.length() > 4) {
				string temp = command.substr(0,4);
				if(temp == "wcc_") {
					scope = LWCC;
					command = command.substr(4, command.length()-4);
				}
				else if(temp == "scc_") {
					scope = LSCC;
					command = command.substr(4, command.length()-4);					
				}
			} 
		
			if(0); 
			
			//////////////////////////////////////////// parameter free commands
			else if(command == "nodes") 
				cout << G.nodes(scope) << endl;
			else if(command == "edges") 
				cout << G.edges(scope) << endl;		
			else if(command == "selfedges") 
				cout << G.selfEdges(scope) << endl;		
			else if(command == "density") 
				cout << G.density(scope) << endl;		
			else if(command == "reciprocity") 
				cout << G.reciprocity(scope) << endl;		
			else if(command == "averagedegree") 
				cout << G.averageDegree(scope) << endl;				
			else if(command == "average_local_clustering" || command == "avg_local_clustering") 
				cout << G.averageClusteringCoefficient(scope) << endl;
			else if(command == "graph_clustering") 
				cout << G.graphClusteringCoefficient(scope) << endl;
			else if(command == "diameter") // wcc only
				cout << G.diameterBD() << endl;
			else if(command == "radius") // wcc only
				cout << G.radiusBD() << endl;
			else if(command == "center") // wcc only
				cout << G.centerSizeBD() << endl;
			else if(command == "periphery") // wcc only
				cout << G.peripherySizeBD() << endl;
			else if(command == "dist_distri") {
				templong = G.distanceDistribution(scope, 1);
				G.printList(templong);	
			}	
			
			///////////////////////////////////////////// parameterized commands			
			else if(command == "distance") {
				cin >> par >> par2;			
				cout << G.distance(atoi(par.c_str()), atof(par2.c_str())) << endl;
			}	
			else if(command == "distances") {
				cin >> par >> par2;		
				tempint = G.alldistances(atoi(par.c_str()));
				G.printPythonNodeList(tempint, scope, par2.c_str());					
			}									

			else if(command == "est_dist_distri") {
				cin >> par;
				templong = G.distanceDistribution(scope, atof(par.c_str()));
				G.printList(templong);			
			}				
			
			
			
			
			// TODO continue migrating stuff below to new format
			
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
			
			else if(command == "adj_list") {
				//cin >> par >> filenamepar;
				//getAdjacencyList(atoi(par.c_str()), filenamepar);		
			}		
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
			
			else if(command == "deg_distri") {
				cin >> par;				
				//printDistri(outDeg, atoi(par.c_str()));
			}					
		
			////////////////////////////////////////////////////////// distances			


		
			// TODO function for average of distri			
		
			// extreme distances
			else if(command == "eccentricity_distri") {
				//tempint = eccentricitiesBD(); 
				//printDistri(tempint, true);
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

		} // if loaded
		
		// loading
		else if(command == "load" || command == "load_undirected") {
			clog << "Loading an undirected graph. Enter filename: ";
			cin >> par;
			if(G.loadUndirected(par)) {
				clog << "Loading file succeeded." << endl;
				G.computeWCC();	
			}
			else
				cerr << "Failed loading file." << endl;
		} 
		else if(command == "load_directed") {
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
		else {
			cerr << "Command not recognized." << endl;
		}				
		
		cerr << "Input a command: ";				
		
	} // while
	clog << endl << "End of program." << endl;
    return 0;
} // listener
