#ifndef CLIENTMAIN_H
# define CLIENTMAIN_H

# include <thread>

# include "Config.h"
# include "Menu.hpp"
# include "TimeHandling.hpp"
# include "Exception.hpp"
# include "Settings.hpp"
# include "Console.hpp"
# include "World.hpp"

class ClientMain
{
public:
  /** Default constructor */
  ClientMain();
  /** Default destructor */
  virtual ~ClientMain();

  void run();

protected:
private:
  void update();

private:
  Settings _settings;
  sf::RenderWindow _window;
  Menu _menu;
  bool _showMenu;
  std::thread _updateThread;
};

#endif // CLIENTMAIN_H
