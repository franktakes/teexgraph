/*
 * teexGraph --- by Frank Takes --- https://github.com/franktakes/teexgraph
 * 
 * Listener for standard input/output dialogs with the code
 */

#include "listener.h"
#include <iostream>
using namespace std;

// listener function
int listener() {
    clog << "*** Welcome to teexGraph ***" << endl <<
            "- Use standard input (cin) to give commands" << endl <<
            "- Read standard output (cout) to catch the result" << endl <<
            "- Observe standard log (clog) and (cerr) for status and error " << 
            "messages" << endl;
            
    // TODO add listener here
    
    return 0;
} // listener
