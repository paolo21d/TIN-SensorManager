#ifndef SERVER_CONNECTIONEXCEPTION_H
#define SERVER_CONNECTIONEXCEPTION_H

#include <exception>

class NetworkException : public std::exception
{
public:
    enum ErrorType
    {
        NON_SPECIFIED,
        BIND,
        LISTEN,
        ACCEPT
    };

    NetworkException(std::string errorMsg);
    NetworkException(ErrorType errorType, std::string errorMsg = "");

    std::string what();
private:
    ErrorType errorType = NON_SPECIFIED;
    std::string errorMsg = "";
};

#endif //SERVER_CONNECTIONEXCEPTION_H
