/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 *
 * Graph class templated functions headerless
 */

// print distribution of values in an array
template <typename Number>
void printDistri(
    const std::vector<Number> & values,
    const Scope scope = Scope::FULL
) {
    std::vector<Number> intarray(n + 1, 0);
    for(size_t i = 0; i < values.size(); i++) {
        if(inScope(i, scope)) {
            intarray[values[i]]++;
        }
    }
    printList(intarray);
} // printDistri


// print an array of values, tab-separated [value frequency]
template <typename Number>
void printList(const std::vector<Number> & values) {
    long double total = 0;
    double count = 0;
    //NOTE: Fails if values.size()==0
    Number minvalue = values.front();
    Number maxvalue = values.front();
    size_t mini = 0;
    size_t maxi = 0;
    for(size_t i = 0; i < values.size(); i++)
        if(values[i] > 0) {
            mini = std::min(i, mini);
            maxi = std::max(i, maxi);
            minvalue = std::min(values[i], minvalue);
            maxvalue = std::max(values[i], maxvalue);
            std::cout << i << '\t' << values[i] << std::endl; // to print any list
            total += i * values[i];
            count += values[i];
        }
    std::clog << "Weighted total: " << total << ", count: " << count <<
            ", average: " << (double) total / count << std::endl;
    std::clog << "Min-index: " << mini << ", max-index: " << maxi <<
            ", min-value: " << minvalue << ", max-value: " << maxvalue << std::endl;
} // printList


// print an array of values, tab-separated [node value]
template <typename Number>
void printNodeList(
    const std::vector<Number> & values,
    const Scope scope = Scope::FULL
) {
    std::vector<int> emptyfilter(n, 0);
    if(scope == Scope::LSCC)
        printFilteredNodeList(values, sccId, largestSCC);
    else if(scope == Scope::LWCC)
        printFilteredNodeList(values, wccId, largestWCC);
    else if(scope == Scope::FULL)
        printFilteredNodeList(values, emptyfilter, 0);
} // printNodeList


// print arrays of values, tab-separated [node value]
template <typename Number>
void printNodeMultiList(
    const std::vector< std::vector<Number> > & values,
    const Scope scope = Scope::FULL
) {
    std::vector<int> emptyfilter(values[0].size(), 0);
    if(scope == Scope::LSCC)
        printFilteredNodeMultiList(values, sccId, largestSCC);
    else if(scope == Scope::LWCC)
        printFilteredNodeMultiList(values, wccId, largestWCC);
    else if(scope == Scope::FULL)
        printFilteredNodeMultiList(values, emptyfilter, 0);
} // printNodeList


// print arrays of values, tab-separated [node value]
template <typename Number>
void printFilteredNodeMultiList(
    const std::vector< std::vector<Number> > & values,
    const std::vector<int> & targetfilter,
    const int targetvalue
) {
    long double count = 0;
    for(size_t i = 0; i < values[0].size(); i++)
        if(targetfilter[i] == targetvalue) {
            std::cout << revMapNode(i);
            for(size_t j = 0; j < values.size(); j++)
                std::cout << '\t' << values[j][i]; // to print node lists
            std::cout << std::endl;
            count += 1;
        }
    std::clog << "Count: " << count << std::endl;
}


// print an array of values, tab-separated [node value]
template <typename Number>
void printFilteredNodeList(
    const std::vector<Number> & values,
    std::vector<int> & targetfilter,
    const int targetvalue
) {
    long double total = 0;
    double count = 0;
    for(int i = 0; i < n; i++)
        if(targetfilter[i] == targetvalue) {
            std::cout << revMapNode(i) << '\t' << values[i] << std::endl; // to print node lists
            total += values[i];
            count += 1;
        }
    std::clog << "Weighted total: " << total << ", count: " << count << ", average: " << (double) total / count << std::endl;
}


// print to binary file for python wrapper output
template <typename Number>
void printPythonNodeList(
    const std::vector<Number> & values,
    const Scope scope = Scope::FULL,
    std::string filename = ""
) {
    //ofstream myFile;
    //myFile.open (filename.c_str(), ios::binary);
    std::clog << "Printing in binary file..." << std::endl;
    FILE *const myFile = fopen(filename.c_str(), "wb");
    Number total = 0;
    double count = 0;
    for(int i = 0; i < n; i++) {
        if(inScope(i, scope)) {
            double tmp2 = values[i];
            //cerr << revMapNode(i) << '\t' << values[i] << std::endl;
            fwrite(&tmp2, sizeof (double), 1, myFile);
        }
    }
    for(int i = 0; i < n; i++) {
        if(inScope(i, scope)) {
            double tmp = 0; // TODO: make stringready. revMapNode(i);
            fwrite(&tmp, sizeof (double), 1, myFile);
            total += values[i];
            count += 1;
        }
    }
    fclose(myFile);
    std::clog << "Weighted total: " << total << ", count: " << count << ", average: " << (double) total / count << std::endl;
}


// print list of nodes according to some filter
template <typename Number>
void printNodes(const std::vector<Number> & targetfilter, const Number targetvalue) {
    for(int i = 0; i < n; i++)
        if(targetfilter[i] == targetvalue) {
            std::cout << revMapNode(i) << std::endl;
        }
}


// print list of edges according to some filter
template <typename Number>
void printEdges(const std::vector<Number> & targetfilter, const Number targetvalue) {
    for(int i = 0; i < n; i++) {
        if(targetfilter[i] == targetvalue) {
            const int z = (signed)E[i].size();
            for(int j = 0; j < z; j++)
                if(targetfilter[E[i][j]] == targetvalue)
                    std::cout << revMapNode(i) << '\t' << revMapNode(E[i][j]) << std::endl;
        }
    }
}
