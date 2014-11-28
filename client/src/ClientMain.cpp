#include "ClientMain.hpp"

ClientMain::ClientMain()
  : _settings(), _parser(&_settings),
    _window(sf::VideoMode(std::stoi(_settings.getCvarList().getCvar("r_width")),
                          std::stoi(_settings.getCvarList().getCvar("r_height"))), "Name"),
    _menu(_settings, _parser),
  _showMenu(false)//,
    //_updateThread(std::bind(&ClientMain::update, this))
{
  _window.setVerticalSyncEnabled(true);
}

ClientMain::~ClientMain()
{
  //  _updateThread.join();
}

void ClientMain::update()
{
  int gfps = 1000 / std::stoi(_settings.getCvarList().getCvar("com_gameFps"));
  TimeHandling time((std::chrono::milliseconds(gfps)));

  ENetEvent	event;
  Network      	socket;

  try
    {
      socket.connect("127.0.0.1", "6060", 2);
      // the updating loop
      time.start();
      while (_window.isOpen())
	{
	  if (socket.pollEvent(&event, 1) < 0)
	    throw NetworkException("Connection problem");
	  switch (event.type)
	    {
	    case ENET_EVENT_TYPE_CONNECT:
	      event.peer->data = (char *)("Server");
	      break;
	    case ENET_EVENT_TYPE_RECEIVE:
	      printf ("A packet of length %lu containing %s was received from %s on channel %u.\n",
		      event.packet->dataLength,
		      event.packet->data,
		      event.peer->data,
		      event.channelID);
	      enet_packet_destroy (event.packet);
	      break;
	    default:
	      break;
	    }
	  socket.sendPacket("Salut", 0);
	  //update object here
	  time.endFrame();
	}
      socket.disconnect();
    }
  catch (NetworkException &e)
    {
      std::cerr << e.what() << std::endl;
    }
}

void ClientMain::run()
{
  Controls	&ctrl = _settings.getControls();
  sf::Event	event;
  t_entry	entry;

  int gfps = 1000 / std::stoi(_settings.getCvarList().getCvar("com_gameFps"));
  TimeHandling time((std::chrono::milliseconds(gfps)));

  time.start();
  //_window.setKeyRepeatEnabled(false);
  while (_window.isOpen())
    {
      _window.clear();
      while (_window.pollEvent(event))
        {
          if (event.type == sf::Event::Closed)
            {
              _window.close();
              break ;
            }
          if (!_menu.run(event, _window, _settings))
            {
	      entry.fill(event);
	      if (event.type == sf::Event::KeyPressed ||
		  event.type == sf::Event::MouseButtonPressed ||
		  event.type == sf::Event::MouseWheelMoved)
                {
		  //std::cout << "keypress" << std::endl;
		  if (event.type == sf::Event::KeyPressed ||
		      event.type == sf::Event::MouseButtonPressed)
		    ctrl.pressKey(entry);
                  // try {
		  //   std::cout << "Action for key " << ctrl.getCodeFromKey(entry) << " is: ";
		  //   }
                  // catch (const Exception &e) {
		  //   std::cout << "Action for key " << "Unknown" << " is: ";
                  //   }
                  // try {
		  //   std::cout << ctrl.getCodeFromAction(ctrl.getActionFromKey(entry)) << std::endl;
                  //   }
                  // catch (const std::out_of_range &oor) {
		  //    std::cout << "Unknown" << std::endl;
                  //   }
                }
              else if (event.type == sf::Event::KeyReleased ||
		       event.type == sf::Event::MouseButtonReleased)
                ctrl.releaseKey(entry);
            }
        }
      //update object here
      time.endFrame();
      // draw stuff here
      _menu.draw(_window);
      _window.display();
    }
}
