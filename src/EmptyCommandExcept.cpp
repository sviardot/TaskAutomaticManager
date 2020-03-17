#include "EmptyCommandExcept.h"

EmptyCommandExcept::EmptyCommandExceptExcept ()
{
    return 0;
}

int EmptyCommandExcept::getErrorNumber() const throw() {
return m_error_number;
}

string EmptyCommandExcept::getErrorMessage() const throw() {
return IDT_EmptyCommandMsg;
}

const char * EmptyCommandExcept::what () const throw ()
    {
    	return IDT_EmptyCommandMsg;
    }
