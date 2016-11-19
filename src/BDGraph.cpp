/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 * 
 * Functions to compute the extreme distance metrics of the WCC of a network.
 * See the following two papers for details:
 * 
 * F.W. Takes and W.A. Kosters, Determining the Diameter of Small World Networks, 
 * in Proceedings of the 20th ACM International Conference on Information and 
 * Knowledge Management (CIKM 2011), pp. 1191-1196, 2011. 
 * http://dx.doi.org/10.1145/2063576.2063748
 * 
 * F.W. Takes and W.A. Kosters, Computing the Eccentricity Distribution of Large Graphs, 
 * Algorithms 6(1): 100-118, 2013. http://dx.doi.org/10.3390/a6010100
 *
 */

#include "BDGraph.h"

// constructor
BDGraph::BDGraph() {
}


// constructor
BDGraph::BDGraph(const int nmax) : Graph(nmax) {

}

// compute the eccentricity of node u - O(m)
int BDGraph::eccentricity(const int u) {
    int current, z, ecc = 0;
    queue<int> q;
    d.assign(nodes(FULL), -1);

    d[u] = 0;
    q.push(u);
    while(!q.empty()) {
        current = q.front();
        q.pop();
        z = neighbors(current).size();
        for(int j = 0; j < z; j++) {
            if(d[neighbors(current)[j]] == -1 && pruned[neighbors(current)[j]] < 0) {
                d[neighbors(current)[j]] = d[current] + 1;
                q.push(neighbors(current)[j]);
                ecc = max(ecc, d[neighbors(current)[j]]);
            }
        }
    }
    return ecc;
} // eccentricity


// compute the diameter of the WCC, brute-force (APSP) - O(mn)
int BDGraph::diameterAPSP() {
    pruned.assign(nodes(FULL), -1);
    computeWCC();
    int diameter = 0;
    cerr << "Computing graph diameter using APSP..." << endl;
    for(int i = 0; i < nodes(FULL); i++) {
        if(i % (nodes(FULL) / 100) == 0) // show status %
            cerr << "\b\b\b\b" << i / (nodes(FULL) / 100) << "%";
        if(inScope(i, LWCC))
            diameter = max(diameter, eccentricity(i));
    }
    cerr << "Diameter computation (APSP) done." << endl << endl;
    return diameter;
} // diameterAPSP


// compute the radius of the WCC, brute-force (APSP) - O(mn)
int BDGraph::radiusAPSP() {
    pruned.assign(nodes(FULL), -1);
    computeWCC();
    int rad = nodes(FULL);
    cerr << "Computing graph radius using APSP..." << endl;
    for(int i = 0; i < nodes(FULL); i++) {
        if(i % (nodes(FULL) / 100) == 0) // show status %
            cerr << "\b\b\b\b" << i / (nodes(FULL) / 100) << "%";
        if(inScope(i, LWCC))
            rad = min(rad, eccentricity(i));
    }
    cerr << "Radius computation (APSP) done." << endl << endl;
    return rad;
} // radiusAPSP


// pruning strategy
int BDGraph::pruning() {
    int z, count = 0, prunee = -1;
    pruned.assign(nodes(FULL), -1);

    // pruned[i] is going to contain the node number that i has identical ecc to
    for(int i = 0; i < nodes(FULL); i++) {
        if(!inScope(i, LWCC))
            continue;

        z = neighbors(i).size();
        prunee = -1;

        for(int j = 0; j < z; j++) {
            if(neighbors(neighbors(i)[j]).size() == 1 && pruned[neighbors(i)[j]] == -1) {
                if(prunee == -1) { // prune all but this one
                    prunee = neighbors(i)[j];
                } else {
                    pruned[neighbors(i)[j]] = prunee; // [0...n-1] indicates that the node was pruned as it is identical to prunee
                    count++;
                    pruned[prunee] = -2; // -2 indicates that its neighbors have been pruned			
                }
            }
        }
    }

    cerr << endl << "Pruning cleared a total of " << count << " nodes." << endl << endl;
    return count;
} // pruning


// compute the graph's WCC diameter using BoundingDiameters
int BDGraph::diameterBD() {
    const bool PRUNE = true;
    return extremaBounding(1, PRUNE);
} // diameterBD


// compute the graph's WCC radius using BoundingDiameters
int BDGraph::radiusBD() {
    const bool PRUNE = true;
    return extremaBounding(2, PRUNE);
} // radiusBD


// get the eccentricities of each of the nodes in the WCC of the graph
vector<int> BDGraph::eccentricitiesBD() {
    const bool PRUNE = true;
    extremaBounding(3, PRUNE);
    return ecc_lower;
} // eccentricitiesBD


// get the eccentricities of each of the nodes in the WCC of the graph brute force
vector<int> BDGraph::eccentricitiesAPSP() {
    pruned.assign(nodes(FULL), -1);
    vector<int> intarray(nodes(FULL), 0);
    for(int i = 0; i < nodes(FULL); i++) {
        intarray[i] = eccentricity(i);
    }
    return intarray;
} // eccentricitiesAPSP

// compute the graph's WCC's periphery using BoundingDiameters
int BDGraph::peripherySizeBD() {
    const bool PRUNE = true;
    return extremaBounding(4, PRUNE);
} // peripheryBD


// compute the graph's WCC's center using BoundingDiameters
int BDGraph::centerSizeBD() {
    const bool PRUNE = true;
    return extremaBounding(5, PRUNE);
} // centerBD


// compute extreme distance values in the WCC using BoundingDiameters
int BDGraph::extremaBounding(const int TYPE = 1, const bool PRUNE = false) {

    if(!isUndirected() || nodes(LWCC) < 2) {
        cerr << "BoundingDiameters is only implemented for undirected graphs' scope LWCC." << endl;
        ecc_lower.assign(nodes(FULL), -1);
        return 0;
    }

    // initialize some values
    int it = 0, current_ecc, current = -1, // n-1 for random
            minupper = -2, maxupper = -1, minlower = -4, maxlower = -3,
            minlowernode = -1, maxuppernode = -1,
            candidates = nodes(LWCC);
    bool showstatus = false;
    ecc_lower.assign(nodes(FULL), 0);
    ecc_upper.assign(nodes(FULL), nodes(LWCC));
    candidate.assign(nodes(FULL), true);
    d.assign(nodes(FULL), 0);

    // initialize high/low selection bool
    bool high = true;

    // perform pruning
    if(PRUNE)
        candidates -= pruning();

    // start the main loop
    while(candidates > 0) {
        ++it;

        // select a random node (much slower, may serve as baseline)
        /*
        do {
                current = rand() % n;
        } while(!candidate[current] || !inScope(current, LWCC) || pruned[current] >= 0);
         */

        // select the next node to be investigated		
        //selectFrom(); // this function has been unrolled here
        high = !high;
        if(current == -1) { // only in the first round, select node with highest degree
            current = 0;
            for(int i = 1; i < nodes(FULL); i++) {
                if(d[i] == -1 || !inScope(i, LWCC) || pruned[i] >= 0)
                    continue;
                if(neighbors(i).size() > neighbors(current).size()) {
                    current = i;
                }
            } // for
        }// if
        else if(high) // select node with highest upper bound
            current = maxuppernode;
        else // select node with lowest lower bound
            current = minlowernode;

        // determine the eccentricity of the current node
        current_ecc = eccentricity(current);

        // output some status info (1)			
        if(TYPE != 3 || candidates % (1 + (nodes(LWCC) / 100)) == 0) {
            cerr << setw(3) << it
                    << ". Current: " << setw(8) << current
                    << " (" << ecc_lower[current] << "/"
                    << ecc_upper[current] << ") -> ";
            showstatus = true;
        }

        // initialize min/max values
        minlower = nodes(LWCC);
        maxlower = 0;
        minupper = nodes(LWCC);
        maxupper = 0;
        maxuppernode = -1;
        minlowernode = -1;

        // update bounds
        for(int i = 0; i < nodes(FULL); i++) {
            if(d[i] == -1 || !inScope(i, LWCC) || pruned[i] >= 0)
                continue;

            // update eccentricity bounds
            ecc_lower[i] = max(ecc_lower[i], max(d[i], current_ecc - d[i]));
            ecc_upper[i] = min(ecc_upper[i], current_ecc + d[i]);

            // update min/max values of lower and upper bounds
            minlower = min(ecc_lower[i], minlower);
            minupper = min(ecc_upper[i], minupper);
            maxlower = max(ecc_lower[i], maxlower);
            maxupper = max(ecc_upper[i], maxupper);
        }

        // update candidate set
        for(int i = 0; i < nodes(FULL); i++) {
            if(!candidate[i] || d[i] == -1 || !inScope(i, LWCC) || pruned[i] >= 0)
                continue;

            // disregard nodes that can no longer contribute
            if(candidate[i] && (
                    (ecc_lower[i] == ecc_upper[i]) || (
                    (TYPE == 1 && ecc_upper[i] <= maxlower && ecc_lower[i]*2 >= maxupper) // diameter
                    || (TYPE == 2 && ecc_lower[i] >= minupper && (ecc_upper[i] + 1) / 2 <= minlower)// radius
                    || (TYPE == 4 && ecc_upper[i] < maxlower && ((maxlower == maxupper) || (ecc_lower[i]*2 > maxupper))) // periphery
                    || (TYPE == 5 && ecc_lower[i] > minupper && ((minlower == minupper) || ((ecc_upper[i] + 1) / 2 < minlower))) // center
                    || (TYPE == 3 && ecc_lower[i] == ecc_upper[i]) // eccentricity distribution			
                    ))) {
                candidate[i] = false;
                candidates--;
            }

            // updating maxuppernode and minlowernode for selection in next round
            if(candidate[i]) {
                if(minlowernode == -1)
                    minlowernode = i;
                else if(ecc_lower[i] == ecc_lower[minlowernode] && neighbors(i).size() > neighbors(minlowernode).size())
                    minlowernode = i;
                else if(ecc_lower[i] < ecc_lower[minlowernode])
                    minlowernode = i;
                if(maxuppernode == -1)
                    maxuppernode = i;
                else if(ecc_upper[i] == ecc_upper[maxuppernode] && neighbors(i).size() > neighbors(minlowernode).size())
                    maxuppernode = i;
                else if(ecc_upper[i] > ecc_upper[maxuppernode])
                    maxuppernode = i;
            }
        }

        // output some status info (2)
        if(TYPE != 3 || showstatus) {
            cerr << setw(3) << current_ecc << " - Bounds: min="
                    << minlower << "/" << minupper << " max=" << maxlower << "/" << maxupper
                    << " - Candidates: " << candidates << endl;
            showstatus = false;
        }
        // current = -1; // do this to re-select a high degree node in the next iteration
    }

    // display number of iterations
    cerr << "\nIterations: ";
    //cout << "\t" << it;
    cerr << "\t" << it;
    cerr << endl << "Number of brute-force iterations: " << nodes(LWCC)
            << ". Speed-up factor: " << (double) nodes(LWCC) / it << endl;

    // return the diameter
    if(TYPE == 1) {
        cerr << "\nDiameter value: ";
        return maxlower;
    }

    // return the radius
    if(TYPE == 2) {
        cerr << "\nRadius value: ";
        return minupper;
    }

    // process ecc values for pruned nodes
    if(PRUNE) {
        for(int i = 0; i < nodes(FULL); i++)
            if(pruned[i] >= 0)
                ecc_lower[i] = ecc_lower[(int) pruned[i]];
    }

    // return the periphery size
    if(TYPE == 4) {
        int periphery = 0;
        for(int i = 0; i < nodes(FULL); i++) {
            if(ecc_lower[i] == maxlower)
                periphery++;
        }
        cerr << "\nPeriphery size: ";
        return periphery;
    }

    // return the center size
    if(TYPE == 5) {
        int center = 0;
        for(int i = 0; i < nodes(FULL); i++) {
            if(ecc_upper[i] == minupper)
                center++;
        }
        cerr << "\nCenter size: ";
        return center;
    }

    // return 0 with the eccentricity distribution in ecc_lower
    cerr << "\nEccentricity distribution: " << endl;
    return 0;
} // extremaBounding


