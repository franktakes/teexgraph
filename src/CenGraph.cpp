/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 *
 * Centrality metrics
 */

#include "Graph.h"

// Compute the closeness of node u - O(m)
double Graph::closeness(const int u) {
    return((double) closenessSum(u)) / ((double) nodesInWcc(wccOf(u)));
} // closeness


// Compute the sum of distances to all other nodes of node u - O(m)
int Graph::closenessSum(const int u) {
    int current, z;
    queue<int> q;
    vector<int> d(nodes(Scope::FULL), -1);

    d[u] = 0;
    q.push(u);
    int total = 0;
    while(!q.empty()) {
        current = q.front();
        q.pop();
        z = neighbors(current).size();
        for(int j = 0; j < z; j++) {
            if(d[neighbors(current)[j]] == -1) {
                d[neighbors(current)[j]] = d[current] + 1;
                q.push(neighbors(current)[j]);
                total += d[neighbors(current)[j]];
            }
        }
    }
    return total;
} // closenessSum


// parallel-ready function for closeness similar to distances()
vector<int> Graph::closenesses(const int u, vector<long> & dtotals) {
    int current, z;
    queue<int> q;
    vector<int> d(nodes(Scope::FULL), -1);

    d[u] = 0;
    q.push(u);
    //dtotals[d[u]]++;
    while(!q.empty()) {
        current = q.front();
        q.pop();
        z = neighbors(current).size();
        for(int j = 0; j < z; j++) {
            if(d[neighbors(current)[j]] == -1) {
                d[neighbors(current)[j]] = d[current] + 1;
                q.push(neighbors(current)[j]);
                dtotals[neighbors(current)[j]] += d[neighbors(current)[j]];
            }
        }
    }
    return d;
} // closenesses


// compute all closeness centrality values in parallel
vector<double> Graph::closenessCentrality(const Scope scope = Scope::LWCC, const double inputsamplesize = 1.0) {

    if(!isUndirected() || scope == Scope::LSCC || nodes(scope) < 2) {
        cerr << "Closeness centrality is only implemented for undirected graphs. Valid scopes are FULL and LWCC." << endl;
        return vector<double>(nodes(Scope::FULL), -1);
    }

    double samplesize = inputsamplesize;
    const int cpus = omp_get_num_procs();
    int tid, a, maxi = nodes(scope);
    double total = 0;
    vector< vector<long> > longarray(cpus, vector<long>(nodes(Scope::FULL), 0));
    vector<bool> done(nodes(Scope::FULL) + 1, false); // for sampling

    if(samplesize < 1.0) {
        maxi = (double) nodes(scope) * samplesize;
    } else if(samplesize > 1.0 && samplesize < nodes(scope)) {
        maxi = samplesize;
        samplesize = (double) maxi / (double) nodes(scope);
    }
    clog << "Computing closeness values (based on a " << samplesize * 100
            << "% sample of " << maxi << " nodes) with " << cpus << " CPUs..." << endl;

#pragma omp parallel for schedule(dynamic, 1) private(tid, a)
    for(int i = 0; i < maxi; i++) {
        tid = omp_get_thread_num();
        if(i % max(1, maxi / 20) == 0) // show status % without div by 0 errors
            clog << " " << i / max(1, maxi / 100) << "%";

        // sampled
        if(samplesize < 1.0) {
            a = nodes(Scope::FULL);
            while(done[a] || !inScope(a, scope)) {
                a = rand() % nodes(Scope::FULL);
            }
            closenesses(a, longarray[tid]);
            done[a] = true;
        }// exact computation
        else {
            if(!inScope(i, scope)) {
                continue;
            }
            closenesses(i, longarray[tid]);
        }
    } // for

    clog << " " << "Done." << endl << endl;

    // merge all cpu results
    vector<long> alllongarray(nodes(Scope::FULL), 0);
    for(int i = 0; i < nodes(Scope::FULL); i++) {
        for(int j = 0; j < cpus; j++) {
            alllongarray[i] += (double) longarray[j][i];
            total += (i * longarray[j][i]);
        }
    }
    vector<double> results(nodes(Scope::FULL), 0.0);
    for(int i = 0; i < nodes(Scope::FULL); i++) {
        results[i] = 1.0 / ((long double) alllongarray[i] / (long double) maxi);
        if(scope == Scope::FULL && !inScope(i, Scope::LWCC))
            results[i] = closeness(i);
    }

    return results;
} // closenessCentrality

// degreeCentrality
vector<double> Graph::degreeCentrality() {
    return outdegreeCentrality();
} // degreeCentrality

// outdegreeCentrality
vector<double> Graph::outdegreeCentrality() {
    vector<double> outDeg(nodes(Scope::FULL)); // outdegrees
    for(int i = 0; i < nodes(Scope::FULL); i++) {
        outDeg[i] = (signed)neighbors(i).size();
    }
    return outDeg;
} // outdegreeCentrallity

// indegreeCentrality
vector<double> Graph::indegreeCentrality() {
    vector<double> inDeg(nodes(Scope::FULL)); // indegrees
    for(int i = 0; i < nodes(Scope::FULL); i++) {
        inDeg[i] = (signed)revNeighbors(i).size();
    }
    return inDeg;
} // indegreeCentrality

// eccentricityCentrality
vector<double> Graph::eccentricityCentrality(const Scope scope = Scope::FULL) {
    if(!isUndirected() || scope == Scope::LSCC || nodes(scope) < 2) {
        cerr << "Eccentricity centrality is only implemented for undirected graphs. Valid scopes are FULL and LWCC." << endl;
        return vector<double>(nodes(Scope::FULL), -1);
    }
    vector<int> eccs = eccentricitiesBD();
    vector<double> centralities(nodes(Scope::FULL), -1);
    for(int i = 0; i < nodes(Scope::FULL); i++)
        if(inScope(i, Scope::LWCC))
            centralities[i] = eccs[i];
        else if(scope == Scope::FULL)
            centralities[i] = eccentricity(i);
    return centralities;
} // eccentricityCentrality

// Compute betweenness centrality cf. Brandes 2001 algorithm
vector<double> Graph::betweennessCentrality(const Scope scope = Scope::FULL, const double samplesize = 1.0) {

    if(!isUndirected() || scope == Scope::LSCC || nodes(scope) < 2) {
        cerr << "Betweenness centrality is only implemented for undirected graphs. Valid scopes are FULL and LWCC." << endl;
        return vector<double>(nodes(Scope::FULL), -1);
    }

    const int cpus = omp_get_num_procs();
    int tid;
    vector< vector<long double> > doublelongarray(cpus, vector<long double>(nodes(Scope::FULL), 0));
    int until = nodes(scope);
    if(samplesize < 1.0)
        until = (double) nodes(Scope::FULL) * samplesize;
    int prevs = -1;

    clog << "Computing betweenness values (based on a " << samplesize * 100
            << "% sample of " << until << " nodes) with " << cpus << " CPUs..." << endl;

#pragma omp parallel for schedule(dynamic, 1) private(tid)
    for(int s = 0; s < until; s++) {
        tid = omp_get_thread_num();

        vector<double> sp;
        vector<int> d;
        vector< vector<int> > P(nodes(Scope::FULL));
        vector<double> delta;
        queue<int> Q;
        stack<int> S;
        int v, w;

        if(s % max(1, until / 20) == 0) // show status % without div by 0 errors
            clog << " " << s / max(1, until / 100) << "%";

        if(samplesize < 1.0) {
            prevs = s;
            do {
                s = rand() % nodes(Scope::FULL);
            } while(!inScope(s, scope));
        } // if

        sp.assign(nodes(Scope::FULL), 0);
        d.assign(nodes(Scope::FULL), nodes(Scope::FULL) + 1);
        P.assign(nodes(Scope::FULL), vector<int>());
        delta.assign(nodes(Scope::FULL), 0);

        sp[s] = 1;
        d[s] = 0;
        Q.push(s);

        while(!Q.empty()) {
            v = Q.front();
            Q.pop();
            S.push(v);

            const int z = (signed)neighbors(v).size();
            for(int i = 0; i < z; i++) {
                int w = neighbors(v)[i];
                if(d[w] == nodes(Scope::FULL) + 1) {
                    d[w] = d[v] + 1;
                    Q.push(w);
                }
                if(d[w] == d[v] + 1) {
                    sp[w] += sp[v];
                    P[w].push_back(v);
                }
            } // for
        } // while

        while(!S.empty()) {
            w = S.top();
            S.pop();
            for(int j = 0; j < (signed)P[w].size(); j++) {
                v = P[w][j];
                delta[v] += ((double) sp[v] / (double) sp[w])*(1.0 + delta[w]);
            }
            if(w != s)
                doublelongarray[tid][w] += (long double) delta[w];
        }

        if(samplesize < 1.0) {
            s = prevs;
        }

    }

    // merge all cpu results
    long double maxval = 0;
    vector<long double> totals(nodes(Scope::FULL), 0);
    for(int i = 0; i < nodes(Scope::FULL); i++) {
        for(int j = 0; j < cpus; j++) {
            totals[i] += (double) doublelongarray[j][i];
        }
        maxval = max(maxval, totals[i]);
    }

    vector<double> centralities(nodes(Scope::FULL), 0);
    for(int i = 0; i < nodes(Scope::FULL); i++) {
        centralities[i] = totals[i] / maxval;
    }

    return centralities;
} // betweennessCentrality


// pageRankCentrality
vector<double> Graph::pageRankCentrality() {

    const double d = 0.85; // dampening factor
    const int iterations = 100;
    double error = 0.0, minval, maxval;
    vector<double> centralities(nodes(Scope::FULL), 1.0 / (double) nodes(Scope::FULL));
    vector<double> centralitiesNext(nodes(Scope::FULL), 0);

    for(int k = 0; k < iterations; k++) {
        // do one iteration
        int z;
        double currentvalue;
        centralitiesNext.assign(nodes(Scope::FULL), 0);

        // compute new pageranks
        for(int i = 0; i < nodes(Scope::FULL); i++) {
            z = (signed)neighbors(i).size();
            currentvalue = centralities[i] / (double) z;
            for(int j = 0; j < z; j++)
                centralitiesNext[neighbors(i)[j]] += currentvalue;
        }

        // compute errors
        error = 0.0;
        minval = 1;
        maxval = 0;
        for(int i = 0; i < nodes(Scope::FULL); i++) {
            error += (centralities[i] - centralitiesNext[i])*(centralities[i] - centralitiesNext[i]);
            centralities[i] = ((1 - d) / nodes(Scope::FULL))+(d * centralitiesNext[i]);
            minval = min(minval, centralities[i]);
            maxval = max(maxval, centralities[i]);
        }

        cerr << k << "% - error: " << setprecision(10) << error << " - value range: " << setprecision(10) << minval << "/" << setprecision(10) << maxval << endl;
    }

    return centralities;
} // pageRankCentrality
