/**
 * \file main.c
 * \brief Main binary.
 * \date 12 august 2014
 *
 * It is the source of the main binary of client or server (one will be needed for both),
 * The infinite loop of the game should be executed here and not in the lib (for testing purpose),
 * Also it would be good that a try catch block do some advanced error reporting.
 *
 */

#include <iostream>
#include <boost/program_options.hpp>

#include "Config.h"
#include "Unused.hpp"
#include "printv.hpp"

#define DEFAULT_PORT 6060

namespace po = boost::program_options;

int parse_argument(int ac, char *av[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "produce help message")
    ("quiet,q", "display nothing")
    ("port", po::value<int>(), "the port of the server")
    ("compression", po::value<int>(), "set compression level")
    ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);
    
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return (1);
    }
    
    if (vm.count("compression")) {
        std::cout << "Compression level was set to "
        << vm["compression"].as<int>() << ".\n";
    } else {
        std::cout << "Compression level was not set.\n";
    }
    
    if (vm.count("port"))
        std::cout << "Actual port => " << vm["port"].as<int>() << std::endl;
    else
        std::cout << "Actual port => " << DEFAULT_PORT << std::endl;
    
    return (0);
}

int	main(int ac, char *av[])
{
    printv(std::cout, "Program version: % %\n", xstr(PROJECT_VERSION), xstr(CURRENT_DATE));
    
    return parse_argument(ac, av);
}
