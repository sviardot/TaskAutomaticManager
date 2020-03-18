#include <iostream>

#include "csystemargument.h"
#include "DateUtility.h"

using namespace std;
using namespace Utility;

int main()
{
    DateUtility dateUtility;

    cout << dateUtility.Now() << endl;
    cout << dateUtility.LogNow() << endl;
    cout << "Hello world!" << endl;
    //cSystemArgument * CallSystem = new cSystemArgument("d:\\temp\\output","d:\\temp\\err","");
    cSystemArgument * CallSystem = new cSystemArgument("d:\\temp\\output","d:\\temp\\err","dir /s d:\\temp\\");
    CallSystem->Run();
    return 0;
}
