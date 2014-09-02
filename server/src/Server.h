//
//  Server.h
//  Project
//
//  Created by Adrien Della Maggiora on 27/08/2014.
//
//

#ifndef Server_H_
#define Server_H_

#include <iostream>

#define DEFAULT_PORT 6060

typedef struct  s_arg
{
    bool        quiet;
    bool        debug;
    int         port;
    s_arg(): quiet(false), debug(false), port(DEFAULT_PORT) {};
}               t_arg;

class Server
{
public:
    
private:
};

#endif
