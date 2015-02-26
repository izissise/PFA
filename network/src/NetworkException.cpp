#include "NetworkException.hpp"

NetworkException::NetworkException(const std::string &str) throw()
: std::runtime_error(str)
{

}

NetworkException::~NetworkException() throw()
{
    
}
