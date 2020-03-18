#include "DateUtility.h"
namespace Utility {
    DateUtility::DateUtility()
    {
        //ctor
    }

    DateUtility::~DateUtility()
    {
        //dtor
    }

    string DateUtility::LogNow()
    {
        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];

        time (&rawtime);
        timeinfo = localtime(&rawtime);

        strftime(buffer,sizeof(buffer),"%Y%m%d-%H%M%S",timeinfo);
        std::string str(buffer);

        return buffer;
    }

    string DateUtility::Now()
    {
        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];

        time (&rawtime);
        timeinfo = localtime(&rawtime);

        strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
        std::string str(buffer);

        return buffer;
    }
}
