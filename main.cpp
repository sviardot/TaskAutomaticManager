#include <iostream>

#include "csystemargument.h"

using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    cSystemArgument * CallSystem = new cSystemArgument("d:\\temp\\output.txt","d:\\temp\\err.txt","dir /s d:\\temp\\");
    return 0;
}
