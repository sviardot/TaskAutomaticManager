#ifndef EMPTYCOMMANDEXCEPT_H
#define EMPTYCOMMANDEXCEPT_H

#include <exception>
#include <string>

using namespace std;

class EmptyCommandExcept : public exception
{
    public:
        int EmptyCommandExceptExcept ();
        int getErrorNumber() const throw();
        string getErrorMessage() const throw();

    protected:
        static const int m_error_number = 255;
        //const string m_error_message = "nom de la commande inconnue ou vide";
    private:
};

#endif // EMPTYCOMMANDEXCEPT_H
