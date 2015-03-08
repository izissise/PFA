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

#include "ServerSettings.hpp"
#include "CvarParser.hpp"
#include "Server.hpp"
#include "Config.h"
#include "Unused.hpp"
#include "printv.hpp"

namespace po = boost::program_options;

int parse_argument(int ac, char *av[], ServerSettings &arg)
{
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("quiet,q", "display nothing")
    ("debug,d", "display debug message")
    ("port", po::value<string>(), "the port of the server")
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
    arg.setCvar("sv_quiet", "1");
  if (vm.count("debug"))
    arg.setCvar("sv_debug", "1");
  if (vm.count("port"))
    arg.setCvar("sv_port", vm["port"].as<string>());
  return (0);
}

int     main(int ac, char *av[])
{
  try
    {
      ServerSettings set;
      CvarParser    parser(set);

      parser.loadConfigFile("../ServerConfig.cfg");
      printv(std::cout, "Program version: % %\n", xstr(PROJECT_VERSION), xstr(CURRENT_DATE));

      if (parse_argument(ac, av, set) == 1)
        return (1);
      Server serv(set);
      serv.run();

      return (0);
    }
  catch (std::exception &e)
    {
      std::cout << e.what() << std::endl;
    }
}
