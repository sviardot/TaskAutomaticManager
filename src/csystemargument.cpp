#include "csystemargument.h"
#include "EmptyCommandExcept.h"

cSystemArgument::cSystemArgument()
{
    //ctor
}

cSystemArgument::cSystemArgument(string OutputStream,string ErrorStream,string CommandName)
{
    if (CommandName.length()>0)
    {
        SetCommandName(CommandName);
        if (OutputStream.length()>0)
            SetOutputStream(OutputStream);
        else
            SetOutputStream(CommandName);
        if (ErrorStream.length()>0)
            SetErrorStream(ErrorStream);
        else
        {
            if (OutputStream.length()>0)
                SetErrorStream(OutputStream);
            else
                SetErrorStream(CommandName);
        }
    }
    else
        throw EmptyCommandExcept();


}

cSystemArgument::~cSystemArgument()
{
    //dtor
}

cSystemArgument::cSystemArgument(const cSystemArgument& other)
{
    //copy ctor
}

cSystemArgument& cSystemArgument::operator=(const cSystemArgument& rhs)
{
    if (this == &rhs)
        return *this; // handle self assignment
    //assignment operator
    return *this;
}
