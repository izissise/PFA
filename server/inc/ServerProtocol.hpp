#ifndef _SERVERPROTOCOL_HPP_
# define _SERVERPROTOCOL_HPP_

class ServerProtocol
{
public:
  ServerProtocol();
  ~ServerProtocol();
  
	void  parseCmd(const void *data, int size);

private:
};

#endif