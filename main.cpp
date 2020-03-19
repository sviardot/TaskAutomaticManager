#include <iostream>
#include <signal.h>
#include <sstream>
#include <string>
#include <cstring>

#include "csystemargument.h"
#include "DateUtility.h"
#include "debug.h"


using namespace std;
using namespace Utility;

void signalHandler( int signum ) {
    cout << "Interrupt signal (" << signum << ") received.\n";
    /*stringstream sstr;
    string str;

    sstr << "Interrupt signal (" << signum << ") received.\n";
    sstr >> str;
    char * cstr = new char [str.length()+1];
    std::strcpy (cstr, str.c_str());
    log_err(cstr);*/
    log_err("signal caught");
   // cleanup and close up stuff here
   // terminate program

   exit(signum);
}

int main()
{
    DateUtility dateUtility;

    log_err("test");
    // register signal SIGINT and signal handler
    signal(SIGINT, signalHandler);

    cout << dateUtility.Now() << endl;
    cout << dateUtility.LogNow() << endl;
    cout << "Hello world!" << endl;
    //cSystemArgument * CallSystem = new cSystemArgument("d:\\temp\\output","d:\\temp\\err","");
    cSystemArgument * CallSystem = new cSystemArgument("d:\\temp\\output","d:\\temp\\err","dir /s d:\\temp\\");
    CallSystem->Run();
    return 0;
}
