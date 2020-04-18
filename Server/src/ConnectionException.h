#ifndef SERVER_CONNECTIONEXCEPTION_H
#define SERVER_CONNECTIONEXCEPTION_H

#include <exception>

class NetworkException : public std::Exception
{
public:
    NetworkException(std::string errorMsg);
    NetworkException(ErrorType errorType, std::string errorMsg = "");

    std::string what() override ;

    enum ErrorType
    {
        BIND,
        LISTEN,
        ACCEPT
    };

private:
    ErrorType errorType
};

#endif //SERVER_CONNECTIONEXCEPTION_H
