#ifndef CSYSTEMARGUMENT_H
#define CSYSTEMARGUMENT_H

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstdio>
#include <iostream>
#include <unistd.h>


using namespace std;

class cSystemArgument
{
    public:
        /** Default constructor */
        cSystemArgument();
        cSystemArgument(string OutputStream,string ErrorStream,string CommandName);
        /** Default destructor */
        virtual ~cSystemArgument();
        /** Copy constructor
         *  \param other Object to copy from
         */
        cSystemArgument(const cSystemArgument& other);
        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        cSystemArgument& operator=(const cSystemArgument& other);

        /** Access m_OutputStream
         * \return The current value of m_OutputStream
         */
        string GetOutputStream() { return m_OutputStream; }
        /** Set m_OutputStream
         * \param val New value to set
         */
        void SetOutputStream(string val) { m_OutputStream = val; }
        /** Access m_ErrorStream
         * \return The current value of m_ErrorStream
         */
        string GetErrorStream() { return m_ErrorStream; }
        /** Set m_ErrorStream
         * \param val New value to set
         */
        void SetErrorStream(string val) { m_ErrorStream = val; }
        /** Access m_CommandName
         * \return The current value of m_CommandName
         */
        string GetCommandName() { return m_CommandName; }
        /** Set m_CommandName
         * \param val New value to set
         */
        void SetCommandName(string val) { m_CommandName = val; }
        void Run();

    protected:

    private:
        string m_OutputStream; //!< Member variable "m_OutputStream"
        string m_ErrorStream; //!< Member variable "m_ErrorStream"
        string m_CommandName; //!< Member variable "m_CommandName"
        char * m_pid;
        vector<string> m_Arguments; //!< Member variable "m_Arguments"
        int fdOut;
        int fdErr;
        char buffer[1024];
};

#endif // CSYSTEMARGUMENT_H
