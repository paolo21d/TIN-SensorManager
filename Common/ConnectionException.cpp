#include "ConnectionException.h"

using namespace std;

ConnectionException::ConnectionException(std::string errorMsg)
{
    this->errorMsg = errorMsg;
}

ConnectionException::ConnectionException(ErrorType errorType, std::string errorMsg)
{
    this->errorType = errorType;
    this->errorType = errorType;
}

string ConnectionException::what()
{
    switch (errorType)
    {
        case NON_SPECIFIED:
            return errorMsg;
        case BIND:
            return "BIND " + errorMsg;
        case LISTEN:
            return "LISTEN " + errorMsg;
        case ACCEPT:
            return "ACCEPT " + errorMsg;
        case RECV:
            return "RECV " + errorMsg;
        case SEND:
            return "SEND " + errorMsg;
        case DATA_LEN:
            return "DATA_LEN " + errorMsg;
        case CREATE_SOCKET:
            return "CREATE_SOCKET: " + errorMsg;
        case SELECT:
            return "SELECT: " + errorMsg;
    }

    return errorMsg;
}