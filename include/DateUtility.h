#ifndef DATEUTILITY_H
#define DATEUTILITY_H

#include <ctime>
#include <string>

using namespace std;

namespace Utility
{
    class DateUtility
    {
    public:
        DateUtility();
        virtual ~DateUtility();
        string Now();
        string LogNow();

    protected:

    private:
    };
};
#endif // DATEUTILITY_H
