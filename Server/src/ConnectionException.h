#ifndef SERVER_CONNECTIONEXCEPTION_H
#define SERVER_CONNECTIONEXCEPTION_H

#include <exception>
#include <string>

class ConnectionException : public std::exception
{
public:
    enum ErrorType
    {
        NON_SPECIFIED,
        BIND,
        LISTEN,
        ACCEPT,
        RECV,
        SEND,
        DATA_LEN,
        CREATE_SOCKET,
        SELECT
    };

    ConnectionException(std::string errorMsg);
    ConnectionException(ErrorType errorType, std::string errorMsg = "");

    std::string what();
private:
    ErrorType errorType = NON_SPECIFIED;
    std::string errorMsg = "";
};

#endif //SERVER_CONNECTIONEXCEPTION_H
