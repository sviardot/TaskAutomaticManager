#include "EmptyCommandExcept.h"

EmptyCommandExcept::EmptyCommandExceptExcept ()
{
    return 0;
}

int EmptyCommandExcept::getErrorNumber() const throw() {
return m_error_number;
}

string EmptyCommandExcept::getErrorMessage() const throw() {
return "nom de la commande inconnue ou vide";
}
