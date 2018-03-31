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
	string outputFile = "temp.bin";
    bool fileOutput = false;

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
				string scopePrefix = command.substr(0,4);
				if(scopePrefix == "wcc_") {
					scope = LWCC;
					command = command.substr(4, command.length()-4);
				}
				else if(scopePrefix == "scc_") {
					scope = LSCC;
					command = command.substr(4, command.length()-4);					
				}
			} 
		
			if(0)
                ; 

			///////////////////////////////////////////////////////////// values
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
			else if(command == "triangles") 
				cout << G.triangles(scope) << endl;
			else if(command == "wedges") 
				cout << G.wedges(scope) << endl;
			else if(command == "averagedegree") 
				cout << G.averageDegree(scope) << endl;				
			else if(command == "average_local_clustering" || 
                    command == "avg_local_clustering") 
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
			else if(command == "distance") {
				cin >> par >> par2;			
				cout << G.distance(atoi(par.c_str()), atoi(par2.c_str())) << endl;
			}
			else if(command == "stats") 
				cerr << "This command is depricated." << endl;	

            /////////////////////////////////////////////////////  distributions				
			else if(command == "wccsize_distri") { 
				tempint = G.wccSizeDistribution();
				G.printDistri(tempint, scope);
			}					
			else if(command == "sccsize_distri") { 
				tempint = G.sccSizeDistribution();
				G.printDistri(tempint, scope);
			}	
			else if(command == "deg_distri" || command == "outdeg_distri") {
				templong = G.outdegreeDistribution(scope);
				G.printDistri(templong, scope);
			}	
			else if(command == "indeg_distri") {
				templong = G.indegreeDistribution(scope);
				G.printDistri(templong, scope);
			}				
			else if(command == "dist_distri") {
				templong = G.distanceDistribution(scope, 1);
				G.printList(templong);	
			}
			else if(command == "est_dist_distri") {
				cin >> par;
				templong = G.distanceDistribution(scope, atof(par.c_str()));
				G.printList(templong);			
			}
			else if(command == "eccentricity_distri") { // wcc only
				tempint = G.eccentricitiesBD(); 
				G.printDistri(tempint, LWCC);
			}	

            ///////////////////////////////////////////////// (Python) nodelists
            else if(command == "local_clustering") {
				tempdouble = G.localClustering(scope);
				if(fileOutput) 
                    G.printPythonNodeList(tempdouble, scope, outputFile);
                else
                    G.printNodeList(tempdouble, scope);
			}	
	
			else if(command == "degree_centrality" || 
                    command == "outdegree_centrality") {
				tempdouble = G.outdegreeCentrality();				
                if(fileOutput) 
                    G.printPythonNodeList(tempdouble, scope, outputFile);
                else
                    G.printNodeList(tempdouble, scope);
			}
			else if(command == "indegree_centrality") {
				tempdouble = G.indegreeCentrality();
				if(fileOutput) 
                    G.printPythonNodeList(tempdouble, scope, outputFile);
                else
                    G.printNodeList(tempdouble, scope);
			}		
			else if(command == "eccentricity_centrality") {
				tempdouble = G.eccentricityCentrality(scope);
				if(fileOutput) 
                    G.printPythonNodeList(tempdouble, scope, outputFile);
                else
                    G.printNodeList(tempdouble, scope);
			}
			else if(command == "closeness_centrality")	{
				tempdouble = G.closenessCentrality(scope, 1);
				if(fileOutput) 
                    G.printPythonNodeList(tempdouble, scope, outputFile);
                else
                    G.printNodeList(tempdouble, scope);
			}
			else if(command == "pagerank_centrality")	{
				tempdouble = G.pageRankCentrality();
				if(fileOutput) 
                    G.printPythonNodeList(tempdouble, scope, outputFile);
                else
                    G.printNodeList(tempdouble, scope);
			}			
			else if(command == "betweenness_centrality")	{
				tempdouble = G.betweennessCentrality(scope, 1);
				if(fileOutput) 
                    G.printPythonNodeList(tempdouble, scope, outputFile);
                else
                    G.printNodeList(tempdouble, scope);
			}
	
			else if(command == "distances") {
				cin >> par;		
				tempint = G.alldistances(atoi(par.c_str()));
				if(fileOutput) 
                    G.printPythonNodeList(tempint, scope, outputFile);
                else
                    G.printNodeList(tempint, scope);
			}			
			else if(command == "est_betweenness_centrality")	{
				cin >> par;
				tempdouble = G.betweennessCentrality(scope, atof(par.c_str()));
				if(fileOutput) 
                    G.printPythonNodeList(tempdouble, scope, outputFile);
                else
                    G.printNodeList(tempdouble, scope);
			}		
			else if(command == "est_closeness_centrality") {
				cin >> par;
				tempdouble = G.closenessCentrality(scope, atof(par.c_str()));
				if(fileOutput) 
                    G.printPythonNodeList(tempdouble, scope, outputFile);
                else
                    G.printNodeList(tempdouble, scope);
			}			
			else if(command == "binary_adj_list" || command == "adj_list") {
				cin >> par;
				G.writeBinaryAdjacencyList(scope, outputFile);		
			}	

            ///////////////////////////////////////////////// change output type

			else if(command == "set_binay_file_output") {
				clog << "Enter 0 for standard cout output or a filename for binary output (nodelists and nodevalues only): ";
				cin >> par;		
                fileOutput = true;
                if(atoi(par.c_str()) == 0)
                    fileOutput = false;
                else
    				outputFile = par;
			}		

            ////////////////////////////////////////////////////////////////////		

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
		
		cerr << "> ";				
		
	} // while
	clog << endl << "End of program." << endl;
    return 0;
} // listener
