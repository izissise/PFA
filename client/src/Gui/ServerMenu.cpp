#include <functional>
#include "ServerMenu.hpp"
#include "ServerInfoPanel.hpp"
#include "printv.hpp"
#include "File.hpp"
#include "StringUtils.hpp"

ServerMenu::ServerMenu(const sf::FloatRect &zone) :
  APanelScreen(zone), _update(true), _masterSocket()
{
  addFont("default", "../client/assets/default.TTF");
  _hide = true;
}

ServerMenu::~ServerMenu()
{
}

void	ServerMenu::construct(const sf::Texture &texture, Settings &set,
			      const std::vector<APanelScreen *> &panels)
{
  _texture = &texture;
  _cont = createContPanel(set, texture, {panels.at(1)}); // pass the gamepanel
  Panel *serv = createServListPanel(set, texture, {_cont});
  Panel *fav = createFavPanel(set, texture, {_cont});
  Widget	*bgWidget = new Widget("bg", _zone, sf::Text(), wFlag::None);
  _cont->addPanel({serv, fav});
  _cont->construct(texture, set, {});
  addPanel(_cont);

  addSpriteForWidget(bgWidget, sf::Color(39, 43, 42, 255), {_zone.width, _zone.height});
  createTabBar(set, texture, {serv, fav});
  Panel *popup = createCoPopup(set, texture, panels);
  createHeader(set, texture, {panels[0], popup});
  createListHeader(set, texture, {_cont});
  _panelCo = createServerPopup(set, texture, {panels.at(1), this});

  // for (unsigned int i = 0; i < 50; ++i)
  //   addServerToList(set, texture, "127.0.0.1:6060", {serv, _cont});

  _widgets.push_back(bgWidget);
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	ServerMenu::parseServerPacket(Settings &set, const void *data, int size)
{
  MasterServerResponse	packet;

  if (packet.ParseFromString(std::string((char *)data, size)))
    {
      if (!packet.has_content())
	return ;
      if (packet.content() == MasterServerResponse::IP)
	{
	  std::lock_guard<std::mutex>	lock(_mutex);
	  const std::vector<APanelScreen *>	&containers = _cont->getSubPanels();
	  const ServerInfo	&server = packet.server();

	  std::cout << "add Server: " << server.ip() << ":" << server.port() << std::endl;
	  _servers.push(t_server(_cont, containers.at(0), server));
	}
    }
  else
    std::cerr << "Cannot DeSerialize Data" << std::endl;
}

void	ServerMenu::updateContent(Settings &set)
{
  const CvarList	&cvars = set.getCvarList();
  ENetEvent		evt;
  bool			connected;

  connected = _masterSocket.isConnected();
  if (!connected)
    _masterSocket.connect(cvars.getCvar("sv_masterIP"),
			  cvars.getCvar("sv_masterPort"),
			  2);
  if (connected)
    {
      std::lock_guard<std::mutex>	lock(_mutex);
      while (!_messages.empty())
	{
	  _masterSocket.sendPacket(0, _messages.front());
	  _messages.pop();
	}
    }
  while ((enet_host_service(_masterSocket.getHost(), &evt, 100)) > 0) // if no evt == 0
    {
      switch (evt.type)
        {
	case ENET_EVENT_TYPE_CONNECT:
	  {
	    std::lock_guard<std::mutex>	lock(_mutex);
	    connected = true;
	    while (!_messages.empty())
	      {
		_masterSocket.sendPacket(0, _messages.front());
		_messages.pop();
	      }
	  }
	  break;
	case ENET_EVENT_TYPE_RECEIVE:
	  parseServerPacket(set, evt.packet->data, evt.packet->dataLength);
	  break;
	default:
	  break;
        }
    }
}

void	ServerMenu::update(std::chrono::milliseconds timeStep, Settings &set)
{
  if (_hide)
    return ;
  if (_update)
    {
      std::lock_guard<std::mutex>	lock(_mutex);
      MasterServerRequest	msg;
      std::string		packet;

      msg.set_content(MasterServerRequest::GETSERVERS);
      msg.set_port("");
      msg.SerializeToString(&packet);
      _messages.push(packet);
      _update = false;
    }
  updateContent(set);
  _update = false;
}

int	ServerMenu::updateHud(const sf::Event &ev, sf::RenderWindow &ref, Settings &set)
{
  int	retVal = 0;
  bool  overlap = _flag & APanelScreen::Display::Overlap;

  if (_state & APanelScreen::State::Inactive)
    {
      if (_countdown.update() == false)
        return 0;
      else
        removeState(APanelScreen::State::Inactive);
    }

  for (auto rit = _panels.rbegin(); rit != _panels.rend(); ++rit)
    {
     if (!(*rit)->isHidden())
        {
          if (!(overlap) || (overlap && checkPanelBounds(*rit)))
            {
              if ((retVal = (*rit)->event(ev, ref, set)) != 0)
                return retVal;
              else if ((*rit)->getState() == APanelScreen::State::Leader)
                return 1;
            }
        }
    }
  for (auto rrrit = _widgets.rbegin(); rrrit != _widgets.rend(); ++rrrit)
    {
     if (checkPanelBounds(*rrrit)) // update widget even if hidden
        if ((retVal = (*rrrit)->update(ev, ref, set)) != 0)
          return retVal;
    }
  return retVal;
}

int	ServerMenu::event(const sf::Event &evt, sf::RenderWindow &ref, Settings &set)
{
  while (!_servers.empty())
    {
      const t_server &s = _servers.front();
      const ServerInfo &packet = s.packet;

      ServerItem *item = addServerToList(set, *_texture, packet.ip() + packet.port(),
					 {s.list, s.container});

      item->updateItem("Name", packet.name());
      item->updateItem("Country", packet.country());
      item->updateItem("Players", std::to_string(packet.currentplayer()) +
		       "/" + std::to_string(packet.maxplayer()));
      item->updateItem("Ping", "0");
      _servers.pop();
    }
  return updateHud(evt, ref, set);
}

void	ServerMenu::createHeader(Settings &set UNUSED,
				 const sf::Texture &texture UNUSED,
				 const std::vector<APanelScreen *> &panels)
{
  Panel		*header = new Panel({_zone.left, _zone.top, _zone.width, 80});
  sf::FloatRect	zone = header->getZone();
  Widget	*wBg = new Widget("bg", zone);
  Widget	*wTitle = new Widget("title", zone,
				     sf::Text("Server List", _font["default"], 30));
  Widget	*wHome = new Widget("home", {0,0,0,0}, // will be cropped
				     sf::Text("Home", _font["default"], 30));
  Widget	*wConnectIp = new Widget("cIp",
					 sf::FloatRect(0,0,0,0),
					 sf::Text("Ip", _font["default"], 30));

  addSpriteForWidget(wBg, sf::Color(27, 32, 26, 255), {zone.width, zone.height});
  createTitle(wTitle, zone);
  createHome(wHome, zone);
  createIp(wConnectIp, zone);
  wHome->addObserver({this, panels[0]});
  wConnectIp->addObserver(panels[1]); // popup
  header->addWidget({wBg, wHome, wTitle, wConnectIp});
  header->construct(texture, set, {});
  addPanel(header);
}

void	ServerMenu::createListHeader(Settings &set, const sf::Texture &texture,
				     const std::vector<APanelScreen *> &panels)
{
  const sf::FloatRect	&listZone = panels[0]->getZone();
  Panel		*cont = new Panel(sf::FloatRect(listZone.left + listZone.width / 6.f,
						listZone.top - 43,
						listZone.width / 2.f, 40));
  const sf::FloatRect	&zone = cont->getZone();
  Widget	*wBg = new Widget("", zone);
  Widget	*wServ = new Widget("", zone, sf::Text("server name", _font["default"], 18));
  Widget	*wCountry = new Widget("", zone, sf::Text("flag", _font["default"], 18));
  Widget	*wPl = new Widget("", zone, sf::Text("players", _font["default"], 18));
  Widget	*wPing = new Widget("", zone, sf::Text("ping", _font["default"], 18));

  addSpriteForWidget(wBg, sf::Color(42, 46, 45, 255), {zone.width, zone.height});
  wServ->alignText({zone.left,zone.top}, {zone.width, zone.height}, 30, 50);
  wCountry->alignText({zone.left,zone.top}, {zone.width, zone.height}, 78, 50);
  wPl->alignText({zone.left,zone.top}, {zone.width, zone.height}, 87.5, 50);
  wPing->alignTextRight({zone.left,zone.top}, {zone.width, zone.height}, 99, 50);
  cont->addWidget({wBg, wServ, wCountry, wPl, wPing});
  for (auto elem : cont->getWidgets())
    elem->setColor(sf::Color(195,190,190, 255));
  cont->construct(texture, set, {});
  addPanel(cont);
}

Panel *ServerMenu::createContPanel(Settings &set UNUSED,
				   const sf::Texture &texture UNUSED,
				   const std::vector<APanelScreen *> &panels)
{
  Panel	*content = new Panel(sf::FloatRect{_zone.left, _zone.top + 140,
	_zone.width, _zone.height - 190});
  std::function<void (const t_event &ev)>	triggerFunc;

  triggerFunc = [content](const t_event &ev)
    {
      if (ev.e & wEvent::Update) // Means a connect to ip
	{
	  content->notify(t_event(wEvent::Hide | wEvent::Toggle)); // swap the panels
	  content->notify(ev); // connect to ip
	}
    };
  content->addObserver({this, panels.at(0)}); // this & gamepanel
  content->setTrigger(triggerFunc);
  return content;
}

Panel	*ServerMenu::createServListPanel(Settings &set, const sf::Texture &texture,
					 const std::vector<APanelScreen *> &panels)
{
  sf::FloatRect	zone = panels[0]->getZone(); // cont's zone
  Panel		*content = new Panel(zone);
  ScrollWidget	*wScroll = new ScrollWidget("scroll",
					    sf::FloatRect(zone.left + zone.width / 2.f +
							  zone.width / 6.f + 8,
							  zone.top, 16, zone.height),
					    Scroll::Vertical, content,
					    sf::Text(), wFlag::None);

  content->setDisplayFlag(APanelScreen::Display::Overlap);
  content->setState(APanelScreen::State::Static);
  createScrollBar(wScroll, texture);
  content->addWidget({wScroll});
  content->construct(texture, set, {});
  return content;
}

// Fav

Panel	*ServerMenu::createFavPanel(Settings &set, const sf::Texture &texture,
				   const std::vector<APanelScreen *> &panels)
{
  sf::FloatRect	zone = panels[0]->getZone();
  Panel		*content = new Panel(zone);
  ScrollWidget	*wScroll = new ScrollWidget("scroll",
					    sf::FloatRect(zone.left + zone.width / 2.f +
							  zone.width / 6.f + 8,
							  zone.top, 16, zone.height),
					    Scroll::Vertical, content,
					    sf::Text(), wFlag::None);

  setFavTrigger(set, texture, content, panels[0]); // container
  content->setDisplayFlag(APanelScreen::Display::Overlap);
  content->setState(APanelScreen::State::Static);
  createScrollBar(wScroll, texture);

  content->addWidget({wScroll});
  content->construct(texture, set, {});
  content->setHide(true);
  return content;
}

void	ServerMenu::setFavTrigger(Settings &set, const sf::Texture &texture,
				  Panel *panel, APanelScreen *container)
{
  std::function<void (const t_event &ev)>  func;

  func = [&, panel, container](const t_event &ev)
    {
      if (ev.e & wEvent::Hide)
  	{
  	  if (ev.e & wEvent::Toggle)
  	    panel->setHide(!(panel->isHidden()));
  	  else
	    panel->setHide(true);
	  if (!panel->isHidden()) // load the panel content
	    loadFavServers(set, texture, panel, container);
	}
      if (ev.e & wEvent::Reset)
	{
	  if (!panel->isHidden()) // refresh the panel content
	    loadFavServers(set, texture, panel, container);
	}
    };
  panel->setTrigger(func);
}

void	ServerMenu::loadFavServers(Settings &set, const sf::Texture &texture,
				   Panel *panel, APanelScreen *container)
{
  std::vector<std::string>	content;
  File		file;
  StringUtils	su;
  std::vector<std::string> serverInfo;

  try
    {
      file.readFile(FavFile, content);
    }
  catch (const std::invalid_argument &e)
    { // Do nothing, file just doesnt exist
    }
  panel->getSubPanels().clear();
  for (const std::string &s : content)
    {
      std::lock_guard<std::mutex>	lock(_mutex);
      MasterServerRequest	msg;
      std::string		packet;
      ServerId			*id = new ServerId;

      su.split(s, ':', serverInfo); // ip:port
      id->set_ip(serverInfo.at(0));
      id->set_port(serverInfo.at(1));
      msg.set_content(MasterServerRequest::GETIP);
      msg.set_port("");
      msg.set_allocated_id(id);
      msg.SerializeToString(&packet);
      _messages.push(packet);
    }
  panel->construct(texture, set, {});
}

void	ServerMenu::trigger(const t_event &ev)
{
  if (ev.e & wEvent::Hide)
    {
      if (ev.e & wEvent::Toggle)
	_hide = !_hide;
      else
	_hide = true;
      if (_hide == false)
	{
	  for (APanelScreen *pan : _panels)
	    pan->setState(APanelScreen::State::Inactive);
	}
    }
  if (ev.e & wEvent::Reset)
    {
      t_event	evt = ev;

      evt.e = static_cast<wEvent>(evt.e & wEvent::None) | wEvent::Reset;
      for (AWidget *widget : _widgets)
	widget->trigger(evt);
    }
}

void	ServerMenu::createTabBar(Settings &set, const sf::Texture &texture,
				 const std::vector<APanelScreen *> &panels)
{
  Panel	*content = new Panel(sf::FloatRect{_zone.left + 90, _zone.top + 140,
	150, 2 * 40});
  sf::FloatRect	zone = content->getZone();
  TabWidget	*tabServer = new TabWidget("bg", {zone.left, zone.top,
					150, 40},
				sf::Text("Server List", _font["default"], 20),
				panels[0]);
  TabWidget	*tabFav = new TabWidget("bg", {zone.left, zone.top + 1 * 40,
					150, 40},
				sf::Text("Favorite List", _font["default"], 20),
				panels[1]);

  createTabButton(tabServer);
  createTabButton(tabFav);
  tabServer->setSpriteAttr(0, false);
  tabServer->setSpriteAttr(1, true);
  tabServer->addObserver(tabFav);
  tabFav->addObserver(tabServer);
  content->addWidget({tabServer, tabFav});
  content->construct(texture, set, {});
  addPanel({content});
}

ServerItem	*ServerMenu::addServerToList(Settings &set,
					     const sf::Texture &texture,
					     const std::string &ip,
					     const std::vector<APanelScreen *> &panels)
{
  APanelScreen	*list = panels[0];
  sf::FloatRect zone = list->getZone();
  sf::FloatRect	widgetZone(zone.left + zone.width / 6.f, 0, zone.width / 2.f, 60);
  unsigned int	nbElem = list->getSubPanels().size();

  widgetZone.top = zone.top + nbElem * widgetZone.height + 3;
  ServerItem *pan = new ServerItem(widgetZone, ip);
  pan->construct(texture, set, {_panelCo});
  list->addPanel(pan);
  return pan;
}

// Server Popup

Panel	*ServerMenu::createServerPopup(Settings &set, const sf::Texture &texture,
				       const std::vector<APanelScreen *> &panels)
{
  Panel	*popup = new Panel(sf::FloatRect{_zone.left + _zone.width / 1.42f,
	_zone.top + 100, 400, 220});
  sf::FloatRect	zone = popup->getZone();
  Widget	*bgWidget = new Widget("bg", zone);
  Widget	*serverName = new Widget("name", sf::FloatRect(zone.left, zone.top,
							       zone.width, 60),
				sf::Text("Server Name", _font["default"], 30));
  Widget	*serverIp = new Widget("", sf::FloatRect(zone.left, zone.top + 60,
							 zone.width / 2, 25),
				       sf::Text("IP Adress:", _font["default"], 20));
  Widget	*ip = new Widget("ip", sf::FloatRect(zone.left + zone.width / 2, zone.top + 60,
							 zone.width / 2, 25),
				       sf::Text("ip", _font["default"], 20));
  Widget	*serverPing = new Widget("", sf::FloatRect(zone.left, zone.top + 90,
							zone.width / 2, 25),
				      sf::Text("Ping:", _font["default"], 20));
  Widget	*ping = new Widget("ping", sf::FloatRect(zone.left + zone.width / 2, zone.top + 90,
							zone.width / 2, 25),
				      sf::Text("0", _font["default"], 20));
  Widget	*serverPl = new Widget("", sf::FloatRect(zone.left, zone.top + 120,
							zone.width / 2, 25),
				      sf::Text("Player Count:", _font["default"], 20));
  Widget	*pl = new Widget("pl", sf::FloatRect(zone.left + zone.width / 2, zone.top + 120,
							zone.width / 2, 25),
				      sf::Text("x/y", _font["default"], 20));

  Widget	*join = new Widget("", sf::FloatRect(zone.left + 10, zone.top + zone.height -
						     60,
						     zone.width - 20, 50),
				   sf::Text("Join Server", _font["default"], 22));

  popup->setHide(true);
  popup->setDisplayFlag(APanelScreen::Display::Overlap);
  addSpriteForWidget(bgWidget, sf::Color(83, 83, 83, 255), {zone.width, zone.height});
  serverName->alignTextLeft({zone.left, zone.top}, {zone.width, 60}, 4.25, 50);
  createPopupText(serverIp, true);
  createPopupText(ip, false);
  createPopupText(serverPing, true);
  createPopupText(ping, false);
  createPopupText(serverPl, true);
  createPopupText(pl, false);
  createPopupJoin(join, ip);
  join->addObserver({panels.at(0), panels.at(1)});
  popup->addWidget({bgWidget, serverName, serverIp, ip, serverPing, ping, serverPl, pl, join});
  setServerPopupTrigger(popup);
  popup->construct(texture, set, {});
  addPanel({popup});
  return popup;
}

void	ServerMenu::createPopupText(Widget *widget, bool left)
{
  const sf::FloatRect &zone = widget->getZone();


  if (left)
    widget->alignTextLeft({zone.left, zone.top}, {zone.width, zone.height}, 9, 50);
  else
    widget->alignTextRight({zone.left, zone.top}, {zone.width, zone.height}, 90, 50);
  widget->setColor(sf::Color(203,200,203,255));
}

void	ServerMenu::createPopupJoin(Widget *widget, Widget *ip)
{
  const sf::FloatRect &zone = widget->getZone();

  widget->alignText({zone.left, zone.top}, {zone.width, zone.height}, 50, 50);
  addSpriteForWidget(widget, sf::Color(113, 139, 68, 255), {zone.width, zone.height});
  widget->setColor(sf::Color(225,225,225,255));

  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [&, ip](AWidget &lwidget, const sf::Event &ev, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      if (isOver)
	lwidget.setColor(sf::Color(255,255,255,255));
      else
	lwidget.setColor(sf::Color(225,225,225,255));
      if (isOver)
	{
	  if (lwidget.isClicked(ev, sf::Mouse::Left))
	    {
	      t_event evt(wEvent::Hide | wEvent::Toggle | wEvent::Update);

	      evt.str = ip->getContent();
	      lwidget.notify(evt);
	      return 1;
	    }
	}
      return 0;
    };
  widget->setUpdate(updateFunc);
}

void	ServerMenu::setServerPopupTrigger(Panel *panel)
{
  std::function<void (const t_event &ev)>  func;

  func = [panel](const t_event &ev) // cannot call APanelScreen::trigger
    {
      if (ev.e & wEvent::Update)
	{
	  StringUtils	su;
	  std::vector<std::string>	info;
	  AWidget *wName = panel->getWidget("name");
	  AWidget *wIp = panel->getWidget("ip");
	  AWidget *wPing = panel->getWidget("ping");
	  AWidget *wPl = panel->getWidget("pl");
	  std::function<void (AWidget *widget)> align = [](AWidget *widget)
	  {
	    const sf::FloatRect &zone = widget->getZone();
	    widget->alignTextRight({zone.left, zone.top}, {zone.width, zone.height}, 90, 50);
	  };
	  const sf::FloatRect &nameZone = wName->getZone();

	  su.split(ev.str, '\n', info);
	  wName->setTextContent(info.at(0));
	  wIp->setTextContent(info.at(1));
	  wPing->setTextContent(info.at(2));
	  wPl->setTextContent(info.at(3));
	  wName->alignTextLeft({nameZone.left, nameZone.top}, {nameZone.width, 60}, 4.25, 50);
	  align(wIp);
	  align(wPing);
	  align(wPl);
	}
      panel->APanelScreen::trigger(ev);
    };
  panel->setTrigger(func);
}

void	ServerMenu::createAddFavButton(const sf::String &ip, Widget *widget,
				       const sf::Texture &texture)
{
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [&](AWidget &lwidget, const sf::Event &ev, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(ev, sf::Mouse::Left))
	    {
	      if (this->addServerToFav(ip))
		{
		  sf::FloatRect zone = lwidget.getZone();

		  lwidget.setTextContent("Done");
		  lwidget.alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
		}
	      // lwidget.notify(t_event(wEvent::Hide | wEvent::Toggle | wEvent::Reset));
	      return 0;
	    }
	}
      return 0;
    };

  std::function<void (const t_event &ev)>  tfunc;

  tfunc = [widget](const t_event &ev) // cannot call APanelScreen::trigger
    {
      if (ev.e & wEvent::Reset)
	{
	  sf::FloatRect zone = widget->getZone();

	  widget->setTextContent("Fav");
	  widget->alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
	}
    };
  createButtonStyle(widget, texture);
  widget->setUpdate(updateFunc);
  widget->setTrigger(tfunc);
}

void	ServerMenu::createRemFavButton(const sf::String &ip, Widget *widget,
				       const sf::Texture &texture)
{
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [&](AWidget &lwidget, const sf::Event &ev, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(ev, sf::Mouse::Left))
	    {
	      if (this->removeServerFromFav(ip))
		{
		  sf::FloatRect zone = lwidget.getZone();

		  lwidget.setTextContent("Done");
		  lwidget.alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
		  lwidget.notify(wEvent::Reset);
		}
	      // lwidget.notify(t_event(wEvent::Hide | wEvent::Toggle | wEvent::Reset));
	      return 0;
	    }
	}
      return 0;
    };

  std::function<void (const t_event &ev)>  tfunc;

  tfunc = [widget](const t_event &ev) // cannot call APanelScreen::trigger
    {
      if (ev.e & wEvent::Reset)
	{
	  sf::FloatRect zone = widget->getZone();

	  widget->setTextContent("unFav");
	  widget->alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
	}
    };
  createButtonStyle(widget, texture);
  widget->setUpdate(updateFunc);
  widget->setTrigger(tfunc);
}

bool	ServerMenu::addServerToFav(const std::string &ip) const
{
  File	file;
  std::vector<std::string> content;

  try
    {
      file.readFile(FavFile, content);
    }
  catch (const std::invalid_argument &e)
    {}
  if (std::find(content.begin(), content.end(), ip) != content.end())
    return false;
  content.push_back(ip);
  file.writeFile(FavFile, content);
  return true;
}

bool	ServerMenu::removeServerFromFav(const std::string &ip) const
{
  File	file;
  std::vector<std::string> content;

  file.readFile(FavFile, content);
  auto it = std::find(content.begin(), content.end(), ip);

  if (it == content.end())
    return false;
  content.erase(it);
  file.writeFile(FavFile, content);
  return true;
}

// -- end

Panel	*ServerMenu::createCoPopup(Settings &set, const sf::Texture &texture,
				   const std::vector<APanelScreen *> &panels)
{
  Panel	*popup = new Panel(sf::FloatRect(_zone.left +
					 _zone.width / 6.f +
					 (_zone.width / 2.f) / 2.f - 150,
					 _zone.top + _zone.height / 2 - 85, 300, 170));
  const sf::FloatRect	&zone = popup->getZone();
  Widget	*bgWidget = new Widget("bg", zone, sf::Text());
  Widget	*header = new Widget("header", {zone.left, zone.top,
					zone.width, 40},
					sf::Text("Connect to Ip", _font["default"], 20));
  TextWidget	*input = new TextWidget("ip", {zone.left + 10, zone.top + 57,
					zone.width - 20, 40},
					sf::Text("", _font["default"], 20),
					sf::Text("Ip", _font["default"], 20), 30);
  Widget	*caButton = new Widget("ca", {zone.left + 10, zone.top + 110,
					zone.width / 2 - 17, 45},
					sf::Text("Cancel", _font["default"], 20));
  Widget	*coButton = new Widget("co", {zone.left + zone.width / 2 + 7,
					zone.top + 110, zone.width / 2 - 17, 45},
					sf::Text("Connect", _font["default"], 20));

  popup->setState(APanelScreen::State::Leader);
  popup->setHide(true);
  addSpriteForWidget(bgWidget, sf::Color(39, 43, 42, 155), {zone.width, zone.height});
  createPopupHeader(header);
  createCancelButton(caButton, texture);
  createConnectButton(coButton, texture);
  createTextWidget(input, texture);
  popup->addObserver({panels[1], this}); // gamePanel
  setPopupTrigger(popup);
  caButton->addObserver(popup);
  coButton->addObserver({popup});
  popup->addWidget({bgWidget, header, input, caButton, coButton});
  popup->construct(texture, set, {});
  addPanel({popup});
  return popup;
}

void	ServerMenu::setPopupTrigger(Panel *panel)
{
  std::function<void (const t_event &ev)>  func;

  func = [panel](const t_event &ev) // cannot call APanelScreen::trigger
    {
      if (ev.e & wEvent::Hide)
	{
	  if (ev.e & wEvent::Toggle)
	    panel->setHide(!panel->isHidden());
	  else
	    panel->setHide(true);
	}
      if (ev.e & wEvent::Update)
	{
	  const std::vector<AWidget *> &widgets = panel->getWidgets();
	  t_event	evt = ev;
	  std::string	ip;

	  for (AWidget *widget : widgets)
	    if (widget->getFlag() & wFlag::Getable)
	      ip = widget->getContent();
	  evt.str = ip;
	  panel->notify(evt);
	}
      if (ev.e & wEvent::Reset)
	{
	  t_event	evt = ev;
	  const std::vector<AWidget *>	&widgets = panel->getWidgets();

	  evt.e = static_cast<wEvent>(evt.e & wEvent::None) | wEvent::Reset;
	  for (AWidget *widget : widgets)
	    widget->trigger(evt);
	}
    };
  panel->setTrigger(func);
}

void	ServerMenu::createTitle(Widget *widget, const sf::FloatRect &zone)
{
  widget->crop();
  widget->setPosition({zone.left, zone.top}, {zone.width, zone.height}, 50, 45);
}

void	ServerMenu::createHome(Widget *widget, const sf::FloatRect &zone)
{
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &lwidget, const sf::Event &ev, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      if (isOver)
	{
	  if (lwidget.isClicked(ev, sf::Mouse::Left))
	    {
	      // so we don't se the flickering when getting back to the menu
	      lwidget.setColor(sf::Color(83, 85, 82, 255));
	      lwidget.notify(t_event(wEvent::Hide | wEvent::Toggle));
	      return 0;
	    }
	  else
	    lwidget.setColor(sf::Color(240, 234, 221, 255));
	}
      else
	lwidget.setColor(sf::Color(83, 85, 82, 255));
      return 0;
    };
  widget->setUpdate(updateFunc);
  widget->crop();
  widget->setPosition({zone.left, zone.top}, {zone.width, zone.height}, 33, 45);
  widget->setColor(sf::Color(83, 85, 82, 255));
}

void	ServerMenu::createIp(Widget *widget, const sf::FloatRect &zone)
{
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &lwidget, const sf::Event &ev, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      if (isOver)
	{
	  if (lwidget.isClicked(ev, sf::Mouse::Left))
	    {
	      lwidget.setColor(sf::Color(83, 85, 82, 255));
	      lwidget.notify(t_event(wEvent::Hide | wEvent::Toggle));
	      return 1;
	    }
	  else
	    lwidget.setColor(sf::Color(240, 234, 221, 255));
	}
      else
	lwidget.setColor(sf::Color(83, 85, 82, 255));
      return 0;
    };
  widget->setUpdate(updateFunc);
  widget->crop();
  widget->setPosition({zone.left, zone.top}, {zone.width, zone.height}, 66, 45);
  widget->setColor(sf::Color(83, 85, 82, 255));
}

void	ServerMenu::createPopupHeader(Widget *widget)
{
  sf::FloatRect zone = widget->getZone();

  addSpriteForWidget(widget, sf::Color(27, 32, 26, 255), {zone.width, zone.height});
  widget->alignText({zone.left, zone.top}, {zone.width, zone.height}, 50, 50);
}

void	ServerMenu::createTabButton(TabWidget *widget)
{
  sf::FloatRect	zone = widget->getZone();

  addSpriteForWidget(widget, sf::Color(59, 63, 62, 255), {zone.width, zone.height});
  addSpriteForWidget(widget, sf::Color(91, 111, 58, 255), {zone.width, zone.height});
  widget->setSpriteAttr(1, false);
  widget->alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
}

void	ServerMenu::createButtonStyle(Widget *widget, const sf::Texture &texture)
{
  sf::FloatRect zone = widget->getZone();

  widget->alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
  widget->addSprite(texture, sf::IntRect(5, 1085, 250, 50));
  addSpriteForWidget(widget, sf::Color(91, 111, 58, 255), {zone.width, zone.height});
  widget->setSpriteAttr(1, false);
  widget->setSpriteSize(0, zone.width, zone.height);
  widget->setEdge(sf::Vector2f(zone.width, zone.height),
		  2.f);
}

void	ServerMenu::createTextWidget(TextWidget *wTextWidget, const sf::Texture &texture UNUSED)
{
  sf::FloatRect	zone = wTextWidget->getZone();
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &widget, const sf::Event &ev UNUSED, sf::RenderWindow &ref UNUSED)
    -> int
    {
      sf::FloatRect wZone = widget.getZone();

      widget.alignText({wZone.left,wZone.top}, {wZone.width, wZone.height}, 50, 50);
      return 0;
    };
  addSpriteForWidget(wTextWidget, sf::Color(255, 255, 255, 200), {zone.width, zone.height});
  wTextWidget->setUpdate(updateFunc);
  wTextWidget->setColor(sf::Color(0,0,0));
  wTextWidget->setDefaultColor(sf::Color(60,60,60));
  wTextWidget->getCursor().setColor(sf::Color(0,0,0));
  wTextWidget->setEdge(sf::Vector2f(zone.width, zone.height),
		       2.f);
}

void	ServerMenu::createCancelButton(Widget *widget, const sf::Texture &texture)
{
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &lwidget, const sf::Event &ev, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(ev, sf::Mouse::Left))
	    {
	      lwidget.notify(t_event(wEvent::Hide | wEvent::Toggle | wEvent::Reset));
	      return 0;
	    }
	}
      return 0;
    };
  createButtonStyle(widget, texture);
  widget->setUpdate(updateFunc);
}

void	ServerMenu::createConnectButton(Widget *widget, const sf::Texture &texture)
{
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &lwidget, const sf::Event &ev, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(ev, sf::Mouse::Left))
	    {
	      lwidget.notify(t_event(wEvent::Hide | wEvent::Toggle | wEvent::Update));
	      return 0;
	    }
	}
      return 0;
    };
  createButtonStyle(widget, texture);
  widget->setUpdate(updateFunc);
}

void	ServerMenu::createScrollBar(ScrollWidget *widget, const sf::Texture &texture)
{
  sf::FloatRect zone = widget->getZone();

  addSpriteForWidget(widget, sf::Color(200, 200, 200, 0), {16, zone.height});
  addSpriteForWidget(widget, sf::Color(100, 100, 100, 255), {16, zone.height});
  // widget->addSprite(texture, sf::IntRect(1012, 1085, 13, 13));
  // widget->addSprite(texture, sf::IntRect(1025, 1085, 13, 13));
  // widget->addSprite(texture, sf::IntRect(1038, 1085, 13, 13));
  widget->toSize(0, 16, zone.height);
  widget->setAutoHide(true);
}
