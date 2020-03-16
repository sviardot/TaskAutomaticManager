#include "csystemargument.h"
#include "EmptyCommandExcept.h"

cSystemArgument::cSystemArgument()
{
    //ctor
}

cSystemArgument::cSystemArgument(string OutputStream,string ErrorStream,string CommandName)
{
        int pid = getpid();
        m_pid = (char *) malloc(6);
        sprintf(m_pid, "%d", pid);

    if (CommandName.length()>0)
    {
        SetCommandName(CommandName);
        if (OutputStream.length()>0)
            SetOutputStream(OutputStream+"-"+m_pid+".lst");
        else
            SetOutputStream(CommandName+"-"+m_pid+".lst");
        if (ErrorStream.length()>0)
            SetErrorStream(ErrorStream+"-"+m_pid+".log");
        else
        {
            if (OutputStream.length()>0)
                SetErrorStream(OutputStream+"-"+m_pid+".log");
            else
                SetErrorStream(CommandName+"-"+m_pid+".log");
        }
    }
    else
        throw EmptyCommandExcept();
    if (OutputStream.length()>0)
        freopen(m_OutputStream.c_str(),"w+",stdout);
    if (ErrorStream.length()>0)
        freopen(m_ErrorStream.c_str(),"w+",stderr);

}

cSystemArgument::~cSystemArgument()
{
    if (m_OutputStream.length()>0)
        fclose(stdout);
    if (m_ErrorStream.length()>0)
        fclose(stderr);

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
