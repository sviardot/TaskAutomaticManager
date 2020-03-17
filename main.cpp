#include <iostream>

#include "csystemargument.h"

using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    //cSystemArgument * CallSystem = new cSystemArgument("d:\\temp\\output","d:\\temp\\err","");
    cSystemArgument * CallSystem = new cSystemArgument("d:\\temp\\output","d:\\temp\\err","dir /s d:\\temp\\");
    CallSystem->Run();
    return 0;
}
