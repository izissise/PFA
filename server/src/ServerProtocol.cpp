#include <iostream>
#include "Protocol.pb.h"
#include "ServerProtocol.hpp"

ServerProtocol::ServerProtocol()
{
  
}

ServerProtocol::~ServerProtocol()
{
  
}

void  ServerProtocol::parseCmd(const void *data, int size)
{
	Protocol	tmp;

    if (tmp.ParseFromString(std::string((char *)data, size)))
		std::cout << tmp.action() << std::endl;
	else
		std::cerr << "Cannot DeSerialize Data" << std::endl;
}

