#ifndef _NETWORKEXCEPTION_HPP_
#define _NETWORKEXCEPTION_HPP_

#include <exception>
#include <iostream>

class NetworkException : public std::runtime_error
{
public:
    explicit NetworkException(const std::string &str) throw();
    virtual ~NetworkException() throw();

private:
};

#endif
