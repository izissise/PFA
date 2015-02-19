#include <chrono>

#include "ServerItem.hpp"
#include "StringUtils.hpp"

ServerItem::ServerItem(const sf::FloatRect &zone, const std::string &ip) :
  APanelScreen(zone), _ip(ip), _socket(), _time()
{
  addFont("default", "../client/assets/default.TTF");
}

void	ServerItem::construct(const sf::Texture &texture, Settings &set,
			      const std::vector<APanelScreen *> &panels)
{
  Widget	*wBg = new Widget("", _zone);
  Widget	*wName = new Widget("Name", sf::FloatRect(_zone.left, _zone.top,
							  600, _zone.height),
				    sf::Text("", _font["default"], 20));
  Widget	*wCountry = new Widget("Country", sf::FloatRect(_zone.left + 600, _zone.top,
								50, _zone.height),
				       sf::Text("", _font["default"], 20));
  Widget	*wPlayers = new Widget("Players", sf::FloatRect(_zone.left + 650, _zone.top,
								100, _zone.height),
				       sf::Text("", _font["default"], 20));
  Widget	*wPing = new Widget("Ping", sf::FloatRect(_zone.left + 750, _zone.top,
							  50, _zone.height),
				    sf::Text("", _font["default"], 20));

  createBackgroundController(wBg);
  wBg->addObserver(panels[0]);
  addWidget({wBg, wName, wCountry, wPlayers, wPing});
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

int	ServerItem::event(const sf::Event &ev, sf::RenderWindow &ref, Settings &set)
{
  {
    std::lock_guard<std::mutex> lock(_mutex);

    while (!_pingTime.empty())
      {
	updateItem("Ping", std::to_string(_pingTime.front()));
	_pingTime.pop();
      }
  }
  return APanelScreen::event(ev, ref, set);
}

void	ServerItem::update(std::chrono::milliseconds timeStep, Settings &set)
{
  ENetEvent		evt;
  bool			connected;
  std::function<void ()> pingFunc =
    [this]()
    {
      ClientMessage	msg;
      PingTime		*ping = new PingTime;
      std::string	packet;

      ping->set_time(std::chrono::duration_cast<std::chrono::milliseconds>
		     (std::chrono::system_clock::now().time_since_epoch()).count());
      msg.set_allocated_ping(ping);
      msg.set_content(ClientMessage::PING);
      msg.SerializeToString(&packet);
      _socket.sendPacket(0, packet);
    };

  if (_time.getElapsedTime().asMilliseconds() < 1000)
    return ;
  connected = _socket.isConnected();
  if (!connected)
    {
      StringUtils		su;
      std::vector<std::string>	serverInfo;

      su.split(_ip, ':', serverInfo); // ip:port
      _socket.connect(serverInfo.at(0),
		      serverInfo.at(1),
		      2);
    }
  _time.restart();
  if (connected)
    pingFunc();
  while ((enet_host_service(_socket.getHost(), &evt, 100)) > 0) // if no evt == 0
    {
      switch (evt.type)
        {
	case ENET_EVENT_TYPE_CONNECT:
	  pingFunc();
	  break;
	case ENET_EVENT_TYPE_RECEIVE:
	  {
	    ClientMessage	packet;

	    if (packet.ParseFromString(std::string((char *)evt.packet->data, evt.packet->dataLength)))
	      {
		if (packet.has_ping())
		  {
		    std::lock_guard<std::mutex> lock(_mutex);
		    uint64_t	time = std::chrono::duration_cast<std::chrono::milliseconds>
		      (std::chrono::system_clock::now().time_since_epoch()).count();
		    _pingTime.push(time - packet.ping().time());
		  }
	      }
	  }
	  break;
	default:
	  break;
        }
    }
}

void	ServerItem::createBackgroundController(Widget *widget)
{
  addSpriteForWidget(widget, sf::Color(59, 63, 62, 255), {_zone.width, _zone.height});
  addSpriteForWidget(widget, sf::Color(91, 111, 58, 255), {_zone.width, _zone.height});
  widget->setSpriteAttr(1, false);
  widget->setEdge(sf::Vector2f(_zone.width, _zone.height), 3.f,
	       sf::Color(46, 50, 49, 255));

  std::function	<int (AWidget &widget, const sf::Event &evt, sf::RenderWindow &ref)>
    updateDisplay =
    [this](AWidget &lwidget, const sf::Event &evt, sf::RenderWindow &ref)
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(evt, sf::Mouse::Right))
	    {
	      t_event	ev(wEvent::Update | wEvent::Hide | wEvent::Toggle);

	      ev.str = getWidget("Name")->getContent() + "\n" +
	      	getIp() + "\n" +
		getWidget("Ping")->getContent() + "\n" +
	      	getWidget("Players")->getContent();
	      lwidget.notify(ev);
	      return 1;
	    }
	  else if (lwidget.isClicked(evt, sf::Mouse::Left))
	    {
	      t_event	ev(wEvent::Update | wEvent::Show);

	      ev.str = getWidget("Name")->getContent() + "\n" +
	      	getIp() + "\n" +
		getWidget("Ping")->getContent() + "\n" +
	      	getWidget("Players")->getContent();
	      lwidget.notify(ev);
	      return 1;
	    }
	}
      return 0;
    };
  widget->setUpdate(updateDisplay);
}

void	ServerItem::updateItem(const std::string &info,
			       const std::string &value)
{
  AWidget	*widget = getWidget(info);
  const sf::FloatRect &zone = widget->getZone();

  widget->setTextContent(value);
  if (info == "Name")
    widget->alignTextLeft({zone.left, zone.top}, {zone.width, zone.height}, 3, 50);
  else
    widget->alignText({zone.left, zone.top}, {zone.width, zone.height}, 50, 50);
}

const std::string	&ServerItem::getIp() const
{
  return _ip;
}
