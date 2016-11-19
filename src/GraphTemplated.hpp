/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 * 
 * Graph class templated functions headerless
 */

// print distribution of values in an array
template <typename Number>
void printDistri(vector<Number> & values, const Scope scope = FULL) {
    vector<Number> intarray(n + 1, 0);
    for(int i = 0; i < (signed)values.size(); i++) {
        if(inScope(i, scope)) {
            intarray[values[i]]++;
        }
    }
    printList(intarray);
} // printDistri


// print an array of values, tab-separated [value frequency]
template <typename Number>
void printList(vector<Number> & values) {
    long double total = 0;
    double count = 0;
    Number minvalue = INT_MAX;
    Number maxvalue = -1;
    int mini = INT_MAX;
    int maxi = -1;
    for(int i = 0; i < (signed)values.size(); i++)
        if(values[i] > 0) {
            mini = min(i, mini);
            maxi = max(i, maxi);
            minvalue = min(values[i], minvalue);
            maxvalue = max(values[i], maxvalue);
            cout << i << '\t' << values[i] << endl; // to print any list
            total += i * values[i];
            count += values[i];
        }
    clog << "Weighted total: " << total << ", count: " << count <<
            ", average: " << (double) total / count << endl;
    clog << "Min-index: " << mini << ", max-index: " << maxi <<
            ", min-value: " << minvalue << ", max-value: " << maxvalue << endl;
} // printList


// print an array of values, tab-separated [node value]
template <typename Number>
void printNodeList(vector<Number> & values, const Scope scope = FULL) {
    vector<int> emptyfilter(n, 0);
    if(scope == LSCC)
        printFilteredNodeList(values, sccId, largestSCC);
    else if(scope == LWCC)
        printFilteredNodeList(values, wccId, largestWCC);
    else if(scope == FULL)
        printFilteredNodeList(values, emptyfilter, 0);
} // printNodeList


// print arrays of values, tab-separated [node value]
template <typename Number>
void printNodeList(vector< vector<Number> > & values, const Scope scope = FULL) {
    vector<int> emptyfilter(n, 0);
    if(scope == LSCC)
        printFilteredNodeList(values, sccId, largestSCC);
    else if(scope == LWCC)
        printFilteredNodeList(values, wccId, largestWCC);
    else if(scope == FULL)
        printFilteredNodeList(values, emptyfilter, 0);
} // printNodeList


// print arrays of values, tab-separated [node value]
template <typename Number>
void printFilteredNodeList(vector< vector<Number> > & values, vector<int> & targetfilter, int targetvalue) {
    long double count = 0;
    for(int i = 0; i < n; i++)
        if(targetfilter[i] == targetvalue) {
            cout << revMapNode(i);
            for(int j = 0; j < (signed)values.size(); j++)
                cout << '\t' << values[j][i]; // to print node lists
            cout << endl;
            count += 1;
        }
    clog << "Count: " << count << endl;
}


// print an array of values, tab-separated [node value]
template <typename Number>
void printFilteredNodeList(vector<Number> & values, vector<int> & targetfilter, int targetvalue) {
    long double total = 0;
    double count = 0;
    for(int i = 0; i < n; i++)
        if(targetfilter[i] == targetvalue) {
            cout << revMapNode(i) << '\t' << values[i] << endl; // to print node lists
            total += values[i];
            count += 1;
        }
    clog << "Weighted total: " << total << ", count: " << count << ", average: " << (double) total / count << endl;
}


// print to binary file for python wrapper output
template <typename Number>
void printPythonNodeList(vector<Number> & values, const Scope scope = FULL, string filename = "") {
    //ofstream myFile;
    //myFile.open (filename.c_str(), ios::binary);
    clog << sizeof (double) << " bytes = size of double" << endl;
    FILE* myFile;
    myFile = fopen(filename.c_str(), "wb");
    Number total = 0;
    double count = 0;
    for(int i = 0; i < n; i++) {
        if(inScope(i, scope)) {
            double tmp2 = values[i];
            //cerr << revMapNode(i) << '\t' << values[i] << endl;
            fwrite(&tmp2, sizeof (double), 1, myFile);
        }
    }
    for(int i = 0; i < n; i++) {
        if(inScope(i, scope)) {
            double tmp = revMapNode(i);
            fwrite(&tmp, sizeof (double), 1, myFile);
            total += values[i];
            count += 1;
        }
    }
    fclose(myFile);
    clog << "Weighted total: " << total << ", count: " << count << ", average: " << (double) total / count << endl;
}


// print list of nodes according to some filter
template <typename Number>
void printNodes(vector<Number> & targetfilter, Number targetvalue) {
    for(int i = 0; i < n; i++)
        if(targetfilter[i] == targetvalue) {
            cout << revMapNode(i) << endl;
        }
}


// print list of edges according to some filter
template <typename Number>
void printEdges(vector<Number> & targetfilter, Number targetvalue) {
    for(int i = 0; i < n; i++) {
        if(targetfilter[i] == targetvalue) {
            const int z = (signed)E[i].size();
            for(int j = 0; j < z; j++)
                if(targetfilter[E[i][j]] == targetvalue)
                    cout << revMapNode(i) << '\t' << revMapNode(E[i][j]) << endl;
        }
    }
}

