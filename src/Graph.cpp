/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 * 
 * Graph class functions
 */

#include "Graph.h"

// initialization of Graph object
Graph::Graph() {
    maxn = MAXN;
    clear();
} // Graph constructor

// initialization of Graph object
Graph::Graph(const int nmax) {
    assert(nmax > 0);
    maxn = nmax;
    clear();
} // Graph constructor

// erase the current Graph object
void Graph::clear() {
    nodeMapping.clear();
    revMapping.clear();
    E.assign(maxn, vector<int>(0));
    rE.assign(maxn, vector<int>(0));
    hasSelfLoop.assign(maxn, false);
    n = m = selfm = nexti = 0;
    loaded = sortedandunique = undirected = doneWCC = doneSCC = false;
    largestWCC = wccs = largestSCC = sccs = 0;
    wccId.assign(n, 0);
    wccNodes.assign(n, 0);
    wccEdges.assign(n, 0);
    sccId.assign(n, 0);
    sccNodes.assign(n, 0);
    sccEdges.assign(n, 0);
} // clear


// map input file node node number to id in range [0,n-1]
int Graph::mapNode(const nodeidtype i) {
    if(nodeMapping.find(i) == nodeMapping.end()) {
        nodeMapping[i] = nexti;
        revMapping[nexti] = i;
        nexti++;
    } // if
    return nodeMapping[i];
} // mapNode


// reverse map node id to original node number
nodeidtype Graph::revMapNode(const int i) {
    return revMapping[i];
} // revMapNode


// add an edge to the graph and update the necessary statistics
bool Graph::addEdge(const int u, const int v) {

    // check if node is within bounds set in Graph.h
    if(u < 0 || u >= maxn || v < 0 || v >= maxn) {
        return false;
    }

    // note if it is a self-loop
    if(u == v) {
        selfm++;
        hasSelfLoop[u] = true;
    }

    // increment edge count
    m++;

    // increment node count if one of the two nodes is first seen
    if(rE[v].size() == 0 && E[v].size() == 0) {
        n++;
    }
    if(v != u && rE[u].size() == 0 && E[u].size() == 0) {
        n++;
    }

    E[u].push_back(v);
    rE[v].push_back(u);

    sortedandunique = false;
    undirected = false;
    doneWCC = false;
    doneSCC = false;
    return true;
} // addEdge


// load an undirected graph from a file in edge list format: [u v]
bool Graph::loadUndirected(const string filename) {
    if(!loadDirected(filename))
        return false;
    makeUndirected();
    return true;
} // loadUndirected


// load a graph from a file in edge list format: [u v]
bool Graph::loadDirected(const string filename) {
    nodeidtype u, v;
    long edgesAdded = 0, edgesSkipped = 0;
    ifstream fin;

    clog << endl << "Loading graph from " << filename << " ..." << endl;

    // check if not already loaded
    if(loaded) {
        cerr << "Error: a graph is already loaded. Clear it first." << endl;
        return false;
    }

    // load the file
    fin.open(filename.c_str());
    if(!fin.is_open()) {
        cerr << "Error: file " << filename << " not found." << endl;
        return false;
    }

    // ignore first lines that do not start with an integer character 
    char c = fin.peek();
    while(!(
            (c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z')
            )) {
        while(c != '\n')
            c = fin.get();
        c = fin.peek();
    }

    // load the edge list
    while(fin >> u >> v) {
        if(m % 10000000 == 0 && m != 0)
            clog << "   - " << m << " edges loaded so far..." << endl;
        if(addEdge(mapNode(u), mapNode(v)))
            edgesAdded++;
        else
            edgesSkipped++;
    }

    clog << "- " << edgesAdded << " edges added (m = " << m << ") in total\n- "
            << edgesSkipped << " edges skipped" << endl;
    if(edgesSkipped - selfm > 0)
        clog << " (out-of-bounds, increase maxn in Graph.h!)";
    clog << "- " << selfm << " self-edges added" << endl;
    clog << endl;

    loaded = true;

    // succesful if we didnt have to skip edges	
    if(edgesSkipped == 0) {
        sortEdgeList();
        clog << "Loading done." << endl << endl;
        return true;
    }
    cerr << "Loading failed." << endl << endl;
    clear();
    return false;
} // loadDirected


// check if there is an edge from a to b - O(log(outdegree(a)))
bool Graph::edge(const int a, const int b) {
    if(!sortedandunique)
        sortEdgeList();
    int first = 0, last = E[a].size() - 1, mid;
    while(first <= last) {
        mid = (first + last) / 2;
        if(b > E[a][mid])
            first = mid + 1;
        else if(b < E[a][mid])
            last = mid - 1;
        else
            return true;
    }
    return false;
} // edge


// check if there is an edge from a to b without requiring sortid list - O(outdegree(a))
bool Graph::edgeSlow(const int a, const int b) {
    const int z = E[a].size();
    for(int j = 0; j < z; j++)
        if(E[a][j] == b)
            return true;
    return false;
} // edgeSlow


// sort edge list so that O(log(outdegree(a))) queries edge(a,b) are possible
void Graph::sortEdgeList() {
    clog << "Sorting edge list..." << endl;
    int removed = 0, begin;
    if(!sortedandunique) {
        m = 0;
        // TODO: make parallel, atomic m and removed update
        for(int i = 0; i < n; i++) {
            sort(E[i].begin(), E[i].end());
            begin = E[i].size();
            E[i].erase(unique(E[i].begin(), E[i].end()), E[i].end());
            sort(rE[i].begin(), rE[i].end());
            rE[i].erase(unique(rE[i].begin(), rE[i].end()), rE[i].end());
            m += (long) E[i].size();
            removed += (E[i].size() - begin);
        }

        sortedandunique = true;
        if(removed > 0)
            clog << "- Total of " << removed << " parallel edges have been removed." << endl;
        clog << "Sorting done." << endl;
    } else
        clog << "Was already done." << endl;
} // sortEdgeList


// check if node is in particular scope
bool Graph::inScope(const int u, const Scope scope) {
    return
    (scope == FULL ||
            (scope == LWCC && doneWCC && wccId[u] == largestWCC) ||
            (scope == LSCC && doneSCC && sccId[u] == largestSCC));
} // inScope


// Get the number of nodes n
int Graph::nodes(const Scope scope = FULL) {
    if(scope == FULL) {
        return n;
    } else if(scope == LWCC && doneWCC) {
        return wccNodes[largestWCC];
    } else if(scope == LSCC && doneSCC) {
        return sccNodes[largestSCC];
    }
    return -1;
} // nodes

// Get the number of nodes in particular WCC
int Graph::nodesInWcc(const int WCCId) {
    if(doneWCC)
        return wccNodes[WCCId];
    return -1;
} // nodes

// Get the number of nodes in particular WCC
int Graph::wccOf(const int u) {
    if(doneWCC)
        return wccId[u];
    return -1;
} // nodes

// Get the number of links m
long Graph::edges(const Scope scope = FULL) {
    if(scope == FULL) {
        return m;
    } else if(scope == LWCC && doneWCC) {
        return wccEdges[largestWCC];
    } else if(scope == LSCC && doneSCC) {
        return sccEdges[largestSCC];
    }
    return -1;
} // edges


// Get the number of self-links
long Graph::selfEdges(const Scope scope = FULL) {
    if(scope == FULL)
        return selfm;
    else if(scope == LWCC && doneWCC) {
        int total = 0;
        for(int i = 0; i < n; i++)
            if(inScope(i, scope) && hasSelfLoop[i])
                total++;
        return total;
    } else if(scope == LSCC && doneSCC) {
        int total = 0;
        for(int i = 0; i < n; i++)
            if(inScope(i, scope) && hasSelfLoop[i])
                total++;
        return total;
    }
    return -1;
} // selfEdges


// make graph undirected (i.e., if (u,v) and !(v,u), then add (u,v))
void Graph::makeUndirected() {
    int z, oldm = m;

    if(undirected) {
        clog << "Graph is already undirected." << endl;
        return;
    }

    clog << "Making graph undirected (m = " << m << ")..." << endl;

    // add all links
    sortedandunique = false;

    for(int i = 0; i < n; i++) {
        z = E[i].size();
        for(int j = 0; j < z; j++)
            E[E[i][j]].push_back(i);
        rE[i].clear();
    } // for

    sortEdgeList(); // needed to remove duplicates introduced in previous step
    doneWCC = doneSCC = false;
    undirected = true;
    if(m != oldm && m != oldm * 2) {
        cerr << "  WARNING: number of edges is not equal to (twice the) number of input lines."
                << endl << "  Verify that the graph is actually undirected." << endl;
    }
    clog << "Undirected-making done (m = " << m << ")." << endl;
} // makeUndirected


// compute to which WCC each node belongs.
void Graph::computeWCC() {

    if(m < 1)
        return;

    wccId.assign(n, 0);
    wccNodes.assign(n, 0);
    wccEdges.assign(n, 0);
    largestWCC = wccs = 0;

    int c, current, z;
    queue<int> q;
    c = 1;

    for(int i = 0; i < n; i++) {
        if(wccId[i] > 0)
            continue;
        while(!q.empty())
            q.pop();
        wccId[i] = c;
        wccNodes[c]++;
        wccEdges[c] += (long) E[i].size() + (long) rE[i].size();
        q.push(i);
        while(!q.empty()) {
            current = q.front();
            q.pop();
            z = E[current].size();
            for(int i = 0; i < z; i++) {
                if(wccId[E[current][i]] == 0) {
                    wccId[E[current][i]] = c;
                    wccNodes[c]++;
                    wccEdges[c] += (long) E[E[current][i]].size() + (long) rE[E[current][i]].size();
                    q.push(E[current][i]);
                }
            }
            if(!undirected) {
                z = rE[current].size();
                for(int i = 0; i < z; i++) {
                    if(wccId[rE[current][i]] == 0) {
                        wccId[rE[current][i]] = c;
                        wccNodes[c]++;
                        wccEdges[c] += (long) E[rE[current][i]].size() + (long) rE[rE[current][i]].size();
                        q.push(rE[current][i]);
                    }
                }
            }
        }

        if(wccNodes[c] > wccNodes[largestWCC])
            largestWCC = c;
        c++;
    }
    c--;

    // because we count both reversed and normal edges we count edges double
    if(!undirected)
        for(int i = 0; i < n; i++)
            wccEdges[i] /= 2;

    wccs = c;
    doneWCC = true;
    clog << "WCC computed." << endl << endl;
} // computeWCC


// helper function for computeSCC(), simulate DFS post-visit marking 
void Graph::goVisitSCC(const int a, vector<bool> & sccVisited,
        stack<int> & sccStack, vector<int> & parentOf, vector<int> & atIt) {

    int current = a, newcurrent;

    while(current != -1) {
        if(atIt[current] == (signed)E[current].size()) {
            sccStack.push(current);
            atIt[current] = 0;
            newcurrent = parentOf[current];
            parentOf[current] = -1;
            current = newcurrent;
        } else {
            if(!sccVisited[E[current][atIt[current]]]) {
                newcurrent = E[current][atIt[current]];
                sccVisited[newcurrent] = true;
                parentOf[newcurrent] = current;
                atIt[current]++;
                current = newcurrent;

            } else {
                atIt[current]++;
            }
        }
    }

} // goVisitSCC


// helper function for computeSCC(), simulate DFS using a stack
void Graph::goMarkSCC(const int x, const int component) {
    stack<int> markStack;
    markStack.push(x);
    int current, z;

    while(!markStack.empty()) {
        current = markStack.top();
        markStack.pop();
        if(current != x)
            sccEdges[component]++;
        if(!sccId[current]) {
            sccId[current] = component;
            sccNodes[component]++;
            z = (signed)rE[current].size();
            for(int i = 0; i < z; i++)
                if(!sccId[rE[current][i]]) {
                    markStack.push(rE[current][i]);
                } else if(sccId[rE[current][i]] == component) {
                    sccEdges[component]++;
                }
        }
    }
} // goMarkSCC 


// find strongly connected components cf. Kosaraju's algorithm, without recursion
void Graph::computeSCC() {

    if(m < 1)
        return;

    if(undirected) {
        cerr << "SCC can only be computed for a directed network." << endl << endl;
        return;
    }

    sccId.assign(n, 0);
    sccNodes.assign(n, 0);
    sccEdges.assign(n, 0);
    largestSCC = sccs = 0;

    vector<bool> sccVisited;
    vector<int> atIt(n, 0), parentOf(n, -1);
    sccVisited.assign(n, false);
    stack<int> sccStack;
    int c = 1;

    for(int i = 0; i < n; i++)
        if(!sccVisited[i] && E[i].size() > 0) {
            sccVisited[i] = true;
            goVisitSCC(i, sccVisited, sccStack, parentOf, atIt);
        } // if

    while(!sccStack.empty()) {
        if(sccId[sccStack.top()] == 0) {
            goMarkSCC(sccStack.top(), c);
            if(sccNodes[c] > sccNodes[largestSCC])
                largestSCC = c;
            c++;
        }
        sccStack.pop();
    } // while
    c--;
    sccs = c;

    doneSCC = true;

    clog << "SCC computed." << endl << endl;
} // computeSCC	


// compute the reciprocity: fraction of links (u,v) that also exist as (v, u) 
double Graph::reciprocity(const Scope scope = FULL) {
    double total = 0, reci = 0;
    for(int i = 0; i < n; i++)
        if(inScope(i, scope)) {
            int z = E[i].size();
            total += z;
            for(int j = 0; j < z; j++) {
                if(edge(E[i][j], i))
                    reci++;
            } // for
        } // if
    if(total > 0)
        return reci / total;
    return 0;
} // reciprocity


// compute the density: the number of edges / the maximum number of edges
double Graph::density(const Scope scope) {
    if(scope == FULL) {
        int noselfnodeswitch = 1;
        if(selfEdges(FULL) > 0)
            noselfnodeswitch = 0;
        if(m > 0 && n > 1)
            return(((double) m) / ((double) n)) / (((double) (n - noselfnodeswitch)));
        return 0;
    } else if((scope == LWCC) & doneWCC) {
        int noselfnodeswitch = 1;
        if(selfEdges(LWCC) > 0)
            noselfnodeswitch = 0;
        if(wccNodes[largestWCC] > 1)
            return(((double) wccEdges[largestWCC]) / ((double) wccNodes[largestWCC])) / (((double) (wccNodes[largestWCC] - noselfnodeswitch)));
        return 0;
    } else if((scope == LSCC) & doneSCC) {
        int noselfnodeswitch = 1;
        if(selfEdges(LSCC) > 0)
            noselfnodeswitch = 0;
        if(sccNodes[largestSCC] > 1)
            return(((double) sccEdges[largestSCC]) / ((double) sccNodes[largestSCC])) / (((double) (sccNodes[largestSCC] - noselfnodeswitch)));
        return 0;
    }
    return -1;
} // density


// average degree in a scope
double Graph::averageDegree(const Scope scope) {
    if(scope == FULL) {
        if(undirected)
            return((double) m / (double) n);
        else
            return((double) m / (double) n) * 2;
    } else if(scope == LWCC && doneWCC) {
        if(undirected)
            return((double) wccEdges[largestWCC] / (double) wccNodes[largestWCC]);
        else
            return((double) wccEdges[largestWCC] / (double) wccNodes[largestWCC]) * 2;
    } else if(scope == LSCC && doneSCC) {
        if(undirected)
            return((double) sccEdges[largestSCC] / (double) sccNodes[largestSCC]);
        else
            return((double) sccEdges[largestSCC] / (double) sccNodes[largestSCC]) * 2;
    }
    return -1;
} // averageDegree


// compute one node's clustering coefficient
double Graph::nodeClusteringCoefficient(const int u) {
    if((signed)E[u].size() > 1) {
        pair<double long, double long> result = trianglesWedgesAround(u);
        if(result.second > 0)
            return(result.first / result.second);
    }
    return 0;
} // nodeClusteringCoefficient


// compute the node clustering coefficient for each node
vector<double> Graph::localClustering(const Scope scope = FULL) {
    vector<double> temparray(n, -1);

    double temp;

#pragma omp parallel for schedule(dynamic, 1) private(temp)
    for(int i = 0; i < n; i++) {
        if(i % max(1, n / 20) == 0) // show status % without div by 0 errors
            clog << " " << i / max(1, n / 100) << "%";
        if(inScope(i, scope)) {
            temp = nodeClusteringCoefficient(i);
            temparray[i] = temp;
        }
    }
    clog << " Done." << endl;

    return temparray;
} // localClustering


// compute the graph's average local clustering coefficient 
double Graph::averageClusteringCoefficient(const Scope scope = FULL) {
    vector<double> values = localClustering(scope);
    long double total = 0;
    for(int i = 0; i < n; i++)
        if(inScope(i, scope))
            total += (long double) values[i];

    if(nodes(scope) > 0)
        return total / (long double) nodes(scope);
    return 0;
} // averageClusteringCoefficient


// get triangle count (ignoring direction)
long Graph::triangles() {
    long grandtotal = 0;
    int tid;
    pair<long, long> result;
    const int cpus = omp_get_num_procs();
    vector<long> total(cpus, 0);
#pragma omp parallel for schedule(dynamic, 1) private(tid,result)
    for(int i = 0; i < n; i++) {
        if(i % max(1, n / 20) == 0) // show status % without div by 0 errors
            clog << " " << i / max(1, n / 100) << "%";
        tid = omp_get_thread_num();
        result = trianglesWedgesAround(i);
        total[tid] += result.first;
    }
    clog << " Done." << endl;
    for(int i = 0; i < cpus; i++)
        grandtotal += total[i];
    return grandtotal / 3;
} // triangles


// get wedge count (ignoring direction)
long Graph::wedges() {
    long grandtotal = 0;
    int tid;
    pair<long, long> result;
    const int cpus = omp_get_num_procs();
    vector<long> total(cpus, 0);
#pragma omp parallel for schedule(dynamic, 1) private(tid,result)
    for(int i = 0; i < n; i++) {
        if(i % max(1, n / 20) == 0) // show status % without div by 0 errors
            clog << " " << i / max(1, n / 100) << "%";
        tid = omp_get_thread_num();
        result = trianglesWedgesAround(i);
        total[tid] += result.second;
    }
    clog << " Done." << endl;
    for(int i = 0; i < cpus; i++)
        grandtotal += total[i];
    return grandtotal;
} // wedges


// compute number of triangles and wedges around a node, ignoring edge direction
pair<long, long> Graph::trianglesWedgesAround(const int u) {
    pair<long, long> result;
    vector<int> templist = E[u];

    // in a directed graph, merge in- and out-neighbors
    if(!undirected) {
        templist.insert(templist.end(), rE[u].begin(), rE[u].end());
        sort(templist.begin(), templist.end());
        templist.erase(unique(templist.begin(), templist.end()), templist.end());
    }

    const int z = templist.size();
    long triangles = 0;

    for(int i = 0; i < z; i++) {
        for(int j = i + 1; j < z; j++)
            if((edge(templist[i], templist[j]) || edge(templist[j], templist[i])) &&
                    templist[i] != u &&
                    templist[j] != u &&
                    templist[i] != templist[j]
                    )
                triangles += 1;
    }

    result.first = triangles;
    int wedgePotential = (signed)templist.size();
    if(hasSelfLoop[u])
        wedgePotential -= 1;
    result.second = (wedgePotential * (wedgePotential - 1)) / 2;
    return result;
} // trianglesWedgesAround


// compute the graph's clustering coefficient: triangles*3/possible triangles
double Graph::graphClusteringCoefficient(const Scope scope = FULL) {
    long double totaltriangles = 0, totalwedges = 0;
    int tid;
    pair<long, long> result;
    const int cpus = omp_get_num_procs();
    vector<long double> triangles(cpus, 0);
    vector<long double> wedges(cpus, 0);
#pragma omp parallel for schedule(dynamic, 1) private(tid,result)             
    for(int i = 0; i < n; i++) {
        if(i % max(1, n / 20) == 0) // show status % without div by 0 errors
            clog << " " << i / max(1, n / 100) << "%";
        if(inScope(i, scope)) {
            tid = omp_get_thread_num();
            result = trianglesWedgesAround(i);
            triangles[tid] += result.first;
            wedges[tid] += result.second;
        }
    }
    clog << " Done." << endl;
    for(int i = 0; i < cpus; i++) {
        totaltriangles += triangles[i];
        totalwedges += wedges[i];
    }
    return totaltriangles / totalwedges;
} // graphClusteringCoefficient


// distribution of weakly connected component sizes
void Graph::wccSizeDistribution() {
    printDistri(wccNodes, LWCC);
    clog << "WCC size distribution printed." << endl;
} // wccSizeDistribution


// distribution of strongly connected component sizes
void Graph::sccSizeDistribution() {
    printDistri(sccNodes, LSCC);
    clog << "SCC size distribution printed." << endl;
} // sccSizeDistribution
void Graph::outdegreeDistribution(const Scope scope = FULL) {
    vector<long> outDeg(n); // outdegrees    
    for(int i = 0; i < n; i++) {
        outDeg[i] = (signed)E[i].size();
    }
    printDistri(outDeg, scope);
    clog << "Outdegree distribution printed." << endl;
} // outdegreeDistribution
void Graph::indegreeDistribution(const Scope scope = FULL) {
    vector<long> inDeg(n); // indegrees
    for(int i = 0; i < n; i++) {
        inDeg[i] = (signed)rE[i].size();
    }
    printDistri(inDeg, scope);
    clog << "Indegree distribution printed." << endl;
} // indegreeDistribution


// compute the distance between node u and v --- O(m) 
int Graph::distance(const int u, const int v) {
    int current, z;
    queue<int> q;
    vector<int> d(n, -1);

    d[u] = 0;
    q.push(u);
    while(!q.empty()) {
        current = q.front();
        q.pop();
        z = E[current].size();
        for(int j = 0; j < z; j++) {
            if(d[E[current][j]] == -1) {
                d[E[current][j]] = d[current] + 1;
                if(E[current][j] == v)
                    return d[v];
                q.push(E[current][j]);
            }
        }
    }
    return d[v];
} // distance


// parallel-ready function to compute all distances and update dtotals
vector<int> Graph::distances(const int u, vector<long> & dtotals) {
    int current, z;
    queue<int> q;
    vector<int> d(nodes(FULL), -1);

    d[u] = 0;
    q.push(u);
    dtotals[d[u]]++;
    while(!q.empty()) {
        current = q.front();
        q.pop();
        z = neighbors(current).size();
        for(int j = 0; j < z; j++) {
            if(d[neighbors(current)[j]] == -1) {
                d[neighbors(current)[j]] = d[current] + 1;
                q.push(neighbors(current)[j]);
                dtotals[d[neighbors(current)[j]]]++;
            }
        }
    }
    return d;
} // distances
double Graph::averageDistance(const Scope scope = FULL, const double inputsamplesize = 1.0) {
    vector<long> result;
    long res = 0;
    result = distanceDistribution(scope, inputsamplesize);
    for(int i = 0; i < nodes(FULL); i++)
        if(result[i] > 0) {
            res += (i * result[i]);
        }
    return res / (long double) ((long double) nodes(scope) * ((long double) nodes(scope) - 1));
} // averageDistance


// print the distance distribution [distance frequency]
vector<long> Graph::distanceDistribution(const Scope scope = FULL, const double inputsamplesize = 1.0) {

    double samplesize = inputsamplesize;

    if(nodes(scope) < 2)
        return vector<long>(1, 0);

    const int cpus = omp_get_num_procs();
    int tid, a, maxi = nodes(scope);
    double total = 0;
    vector< vector<long> > longarray(cpus, vector<long>(n, 0));
    vector<bool> done(n + 1, false); // for sampling
    int donecount = 0;
    if(samplesize < 1.0) {
        maxi = (double) nodes(scope) * samplesize;
    } else if(samplesize > 1.0 && samplesize < nodes(scope)) {
        maxi = samplesize;
        samplesize = (double) maxi / (double) nodes(scope);
    }
    clog << "Computing distance distribution (based on a " << samplesize * 100
            << "% sample of " << maxi << " nodes) with " << cpus << " CPUs..." << endl;

#pragma omp parallel for schedule(dynamic, 1) private(tid, a)
    for(int i = 0; i < n; i++) {
        if(donecount >= maxi)
            continue;
        tid = omp_get_thread_num();
        if(donecount % max(1, maxi / 20) == 0) // show status % without div by 0 errors
            clog << " " << donecount / max(1, maxi / 100) << "%";

        // sampled 
        if(samplesize < 1.0) {
            a = n;
            while(done[a] || !inScope(a, scope)) {
                a = rand() % n;
            }
            distances(a, longarray[tid]);
            donecount++;
            done[a] = true;
        }// exact computation
        else {
            if(!inScope(i, scope)) {
                continue;
            }
            distances(i, longarray[tid]);
            donecount++;
        }

    } // for

    clog << " " << "Done." << endl;

    // merge all cpu results
    vector<long> alllongarray(n, 0);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < cpus; j++) {
            alllongarray[i] += (1.0 / samplesize)*(double) longarray[j][i];
            total += (i * longarray[j][i]);
        }
    }

    //printList(alllongarray);

    return alllongarray;
} // distanceDistribution

// return reference to vector with neighbors of a node
vector<int> & Graph::neighbors(const int i) {
    return E[i];
} // neighbors

// return reference to vector with reversed neighbors of a node
vector<int> & Graph::revNeighbors(const int i) {
    return rE[i];
} // revNeighbors

// check if graph is undirected
bool Graph::isUndirected() {
    return undirected;
} // isUndirected

// check if graph is sorted and unique
bool Graph::isSortedAndUnique() {
    return sortedandunique;
} // isSortedAndUnique

// check if graph's wcc is computed
bool Graph::wccComputed() {
    return doneWCC;
} // wccComputed

// check if graph's scc is computed
bool Graph::sccComputed() {
    return doneSCC;
} // sccComputed

// return number of wccs
int Graph::wccCount() {
    return wccs;
} // wccCount

// return number of sccs
int Graph::sccCount() {
    return sccs;
} // sccComputed