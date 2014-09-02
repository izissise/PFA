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

#include "Server.h"
#include "Config.h"
#include "Unused.hpp"
#include "printv.hpp"

namespace po = boost::program_options;

int parse_argument(int ac, char *av[], t_arg &arg)
{
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "produce help message")
    ("quiet,q", "display nothing")
    ("debug,d", "display debug message")
    ("port", po::value<int>(), "the port of the server")
    ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);
    
    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return (1);
    }
    if (vm.count("quiet"))
        arg.quiet = true;
    if (vm.count("debug"))
        arg.quiet = true;
    if (vm.count("port"))
        arg.port = vm["port"].as<int>();
    return (0);
}

int	main(int ac, char *av[])
{
    t_arg arg;
    printv(std::cout, "Program version: % %\n", xstr(PROJECT_VERSION), xstr(CURRENT_DATE));
    
    if (parse_argument(ac, av, arg) == 1)
        return (1);
    std::cout << "Actual port => " << arg.port << std::endl;
    std::cout << "Quiet => " << arg.quiet << std::endl;
    std::cout << "Debug => " << arg.debug << std::endl;
}
