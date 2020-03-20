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

    fdOut = dup(STDOUT_FILENO); //save the stdout state
    fdErr = dup(STDERR_FILENO);
}

void cSystemArgument::Run()
{
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];

    stream = popen(m_CommandName.c_str(), "r");
    if (stream) {
        while (!feof(stream))
        if (fgets(buffer, max_buffer, stream) != NULL)
            fprintf(stdout,"%s",buffer);
    pclose(stream);
    }
    if (m_OutputStream.length()>0)
        fclose(stdout);
    if (m_ErrorStream.length()>0)
        fclose(stderr);

    freopen("NUL", "a", stdout);
    setvbuf(stdout, buffer, _IOFBF, 1024);
    freopen("NUL", "a", stdout); //redirect stdout to null again
    dup2(fdOut, STDOUT_FILENO); //restore the previous state of stdout
    setvbuf(stdout, NULL, _IONBF, 1024);//disable buffer to print to screen instantly
    #ifdef WIN32
    freopen("CONOUT$", "w", stdout);
    #endif
};

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
