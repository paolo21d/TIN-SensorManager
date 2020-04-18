#include "ConnectionException.h"

using namespace std;

NetworkException::NetworkException(std::string errorMsg)
{
    this->errorMsg = errorMsg;
}

NetworkException::NetworkException(ErrorType errorType, std::string errorMsg)
{
    this->errorType = errorType;
    this->errorType = errorType;
}

string NetworkException::what()
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
    }

    return errorMsg;
}