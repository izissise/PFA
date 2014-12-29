#include <functional>
#include "ServerMenu.hpp"
#include "ServerInfoPanel.hpp"
#include "printv.hpp"
#include "File.hpp"

ServerMenu::ServerMenu(const sf::FloatRect &zone) :
  APanelScreen(zone)
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
  Widget	*wTitle = new Widget("title", {_zone.left, _zone.top, _zone.width, 140},
				     sf::Text("Server List", _font["default"], 40));
  Widget	*wFooter = new Widget("Footer", {_zone.left, _zone.height - 70, _zone.width, 70},
				      sf::Text());
  Widget	*wBack = new Widget("Back", {_zone.left + 10, _zone.height - 57,
					(_zone.width - 60) / 3, 44},
					sf::Text("Back to Menu", _font["default"], 20));
  Widget	*wConnectIp = new Widget("cIp", {_zone.left + 10 + (_zone.width - 60) / 3 + 20,
	_zone.height - 57, (_zone.width - 60) / 3, 44},
    sf::Text("Connect to Ip", _font["default"], 20));
  Widget	*wJoin = new Widget("join", {_zone.left + 10 +  2 * ((_zone.width - 60) / 3 + 20),
	_zone.height - 57, (_zone.width - 60) / 3, 44},
    sf::Text("Join Server", _font["default"], 20));

  createTitle(wTitle);

  Panel *cont = createContPanel(set, texture, {panels.at(1)}); // pass the gamepanel
  Panel *serv = createServListPanel(set, texture, {cont});
  Panel *fav = createFavPanel(set, texture, {cont});
  cont->addPanel({serv, fav});
  cont->construct(texture, set, {});
  addPanel(cont);

  createTabBar(set, texture, {serv, fav});
  Panel *popup = createCoPopup(set, texture, panels);
  Panel	*serverPopup = createServerPopup(set, texture, {cont, fav});
  wConnectIp->addObserver(popup);
  createFooter(wFooter);
  createButtonBack(wBack, texture);
  createButtonCip(wConnectIp, texture);
  createButtonJoin(wJoin, texture);
  wBack->addObserver({this, panels[0]});
  wJoin->addObserver({this, panels[1]});

  for (unsigned int i = 0; i < 50; ++i)
    addServerToList(set, texture, "127.0.0.1", {serv, cont});

  _widgets.push_back(wTitle);
  _widgets.push_back(wFooter);
  _widgets.push_back(wBack);
  _widgets.push_back(wConnectIp);
  _widgets.push_back(wJoin);
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

Panel *ServerMenu::createContPanel(Settings &set, const sf::Texture &texture,
				   const std::vector<APanelScreen *> &panels)
{
  Panel	*content = new Panel(sf::FloatRect{_zone.left, _zone.top + 140,
	_zone.width, _zone.height - 210});
  sf::FloatRect zone = content->getZone();
  std::function<void (const t_event &event)>	triggerFunc;

  triggerFunc = [content](const t_event &event)
    {
      if (event.e & wEvent::Update) // Means a connect to ip
	{
	  content->notify(t_event(wEvent::Hide | wEvent::Toggle)); // swap the panels
	  content->notify(event); // connect to ip
	}
    };
  content->addObserver({this, panels.at(0)}); // this & gamepanel
  content->setTrigger(triggerFunc);
  return content;
}

Panel	*ServerMenu::createServListPanel(Settings &set, const sf::Texture &texture,
					 const std::vector<APanelScreen *> &panels)
{
  sf::FloatRect	zone = panels[0]->getZone();
  Panel		*content = new Panel(zone);
  Widget	*bgWidget = new Widget("bg", {zone.left, zone.top,
				zone.width, zone.height}, sf::Text());
  ScrollWidget	*wScroll = new ScrollWidget("scroll",
					    {zone.left + zone.width - 13, zone.top, 13, zone.height},
					    Scroll::Vertical, content,
					    sf::Text(), wFlag::None);

  content->setDisplayFlag(APanelScreen::Display::Overlap);
  content->setState(APanelScreen::State::Static);
  createScrollBar(wScroll, texture);

  addSpriteForWidget(bgWidget, sf::Color(200, 200, 200, 255), {zone.width, zone.height});
  content->addWidget({bgWidget, wScroll});
  content->construct(texture, set, {});
  return content;
}

// Fav

Panel	*ServerMenu::createFavPanel(Settings &set, const sf::Texture &texture,
				   const std::vector<APanelScreen *> &panels)
{
  sf::FloatRect	zone = panels[0]->getZone();
  Panel		*content = new Panel(zone);
  Widget	*bgWidget = new Widget("bg", {zone.left, zone.top,
				zone.width, zone.height}, sf::Text());
  ScrollWidget	*wScroll = new ScrollWidget("scroll",
					    {zone.left + zone.width - 13, zone.top, 13, zone.height},
					    Scroll::Vertical, content,
					    sf::Text(), wFlag::None);

  setFavTrigger(set, texture, content, panels[0]); // container
  content->setDisplayFlag(APanelScreen::Display::Overlap);
  content->setState(APanelScreen::State::Static);
  createScrollBar(wScroll, texture);

  addSpriteForWidget(bgWidget, sf::Color(200, 200, 200, 255), {zone.width, zone.height});
  content->addWidget({bgWidget, wScroll});
  content->construct(texture, set, {});
  content->setHide(true);
  return content;
}

void	ServerMenu::setFavTrigger(Settings &set, const sf::Texture &texture,
				  Panel *panel, APanelScreen *container)
{
  std::function<void (const t_event &event)>  func;

  func = [&, panel, container](const t_event &event)
    {
      if (event.e & wEvent::Hide)
  	{
  	  if (event.e & wEvent::Toggle)
  	    panel->setHide(!(panel->isHidden()));
  	  else
	    panel->setHide(true);
	  if (!panel->isHidden()) // load the panel content
	    loadFavServers(set, texture, panel, container);
	}
      if (event.e & wEvent::Reset)
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
  File	file;

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
      addServerToList(set, texture, s, {panel, container});
    }
  panel->construct(texture, set, {});
}

Panel	*ServerMenu::createServerPanel(Settings &set, const sf::Texture &texture,
				       const std::vector<APanelScreen *> &panels,
				       const sf::FloatRect &zone,
				       const std::string &ip)
{
  Panel			*controler = new Panel(zone);
  ServerInfoPanel	*serverInfoPanel = new ServerInfoPanel(zone, ip);
  Panel			*style = new Panel(zone);
  Widget		*wBg = new Widget("bg", zone);

  createPopupControler(wBg, panels);
  wBg->addObserver(controler);		// to signal the events
  setControlerTrigger(controler);
  style->addWidget(wBg);
  style->construct(texture, set, {});
  serverInfoPanel->construct(texture, set, {});
  controler->addPanel({style, serverInfoPanel});
  controler->construct(texture, set, {});
  return controler;
}

void	ServerMenu::createPopupControler(Widget *widget,
					 const std::vector<APanelScreen *> &panels)
{
  unsigned int		nbElem = 0;
  sf::FloatRect		zone = widget->getZone();
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateDisplay =
    [](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(event, sf::Mouse::Right))
	    {
	      lwidget.notify(t_event(wEvent::Update | wEvent::Hide | wEvent::Toggle));
	      return 1;
	    }
	  else if (lwidget.isClicked(event, sf::Mouse::Left))
	    {
	      lwidget.notify(t_event(wEvent::Update));
	      return 1;
	    }
	}
      return 0;
    };

  for (APanelScreen *pan : panels[0]->getSubPanels()) // the container
    ++nbElem;
  widget->setUpdate(updateDisplay);
  if (nbElem % 2)
    addSpriteForWidget(widget, sf::Color(100, 100, 100, 255), {zone.width, zone.height});
  else
    addSpriteForWidget(widget, sf::Color(130, 130, 130, 255), {zone.width, zone.height});
  addSpriteForWidget(widget, sf::Color(0x40, 0x6F, 0x39, 255), {zone.width, zone.height});
  widget->setSpriteAttr(1, false);
}

void	ServerMenu::setControlerTrigger(Panel *panel)
{
  std::function<void (const t_event &event)>  func;

  func = [panel](const t_event &event) // cannot call APanelScreen::trigger
    {
      if (event.e & wEvent::Update)
	{
	  const ServerInfoPanel *info = dynamic_cast<ServerInfoPanel *>
	  (panel->getSubPanels().at(1));
	  if (event.e & wEvent::Hide) // right click
	    {
	      t_event	evt;
	      std::ostringstream	newLine;

	      printv(newLine, "%\n%", info->getWidget("Name")->getContent(), info->getIp());
	      evt.str = newLine.str();
	      evt.e = wEvent::UpdateText;
	      panel->notify(evt);
	      panel->notify(t_event(wEvent::Hide | wEvent::Toggle));
	    }
	  else // left click, connect to the game
	    {
	      t_event	connectEvent;

	      connectEvent = event;
	      connectEvent.e = wEvent::Update;
	      connectEvent.str = info->getIp();
	      panel->notify(connectEvent); // send him the ip to connect to;
	    }
	}
    };
  panel->setTrigger(func);
}

void	ServerMenu::trigger(const t_event &event)
{
  if (event.e & wEvent::Hide)
    {
      if (event.e & wEvent::Toggle)
	_hide = !_hide;
      else
	_hide = true;
      if (_hide == false)
	{
	  for (APanelScreen *pan : _panels)
	    pan->setState(APanelScreen::State::Inactive);
	}
    }
  if (event.e & wEvent::Reset)
    {
      t_event	evt = event;

      evt.e = static_cast<wEvent>(evt.e & wEvent::None) | wEvent::Reset;
      for (AWidget *widget : _widgets)
	widget->trigger(evt);
    }
}

void	ServerMenu::createTabBar(Settings &set, const sf::Texture &texture,
				 const std::vector<APanelScreen *> &panels)
{
  Panel	*content = new Panel(sf::FloatRect{_zone.left, _zone.top + 100,
	_zone.width, 40});
  sf::FloatRect	zone = content->getZone();
  TabWidget	*tabServer = new TabWidget("bg", {zone.left, zone.top,
					150, zone.height},
				sf::Text("Server List", _font["default"], 20),
				panels[0]);
  TabWidget	*tabFav = new TabWidget("bg", {zone.left + 150, zone.top,
					150, zone.height},
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

void	ServerMenu::addServerToList(Settings &set,
				    const sf::Texture &texture,
				    const std::string &ip,
				    const std::vector<APanelScreen *> &panels)
{
  APanelScreen	*list = panels[0];
  sf::FloatRect zone = list->getZone();
  sf::FloatRect	widgetZone(zone.left, 0, zone.width - 13, 30);
  // -13 for the scrollbar so it doesnt overlap
  unsigned int	nbElem = 0;

  for (APanelScreen *pan : list->getSubPanels())
    ++nbElem;
  widgetZone.top = zone.top + nbElem * widgetZone.height;
  Panel *pan = createServerPanel(set, texture, {list},
				 widgetZone,
				 "127.0.0.1:6060");
  pan->addObserver({_panels.at(_panels.size() - 1), panels.at(1)}); // serverPopup , contPanel
  list->addPanel(pan);
}

// Server Popup

Panel	*ServerMenu::createServerPopup(Settings &set, const sf::Texture &texture,
				       const std::vector<APanelScreen *> &panels)
{
  Panel	*popup = new Panel(sf::FloatRect{_zone.left + _zone.width / 2 - 200,
	_zone.top + _zone.height / 2 - 110, 400, 220});
  sf::FloatRect	zone = popup->getZone();
  Widget	*bgWidget = new Widget("bg", {zone.left + 2, zone.top,
				zone.width - 4, zone.height}, sf::Text());
  Widget	*header = new Widget("header", {zone.left, zone.top,
					zone.width, 40},
					sf::Text("Server's Options", _font["default"], 20));

  Widget	*serverName = new Widget("serverName", {zone.left + 10, zone.top + 50,
				zone.width / 2, 25},
				sf::Text("Server Name", _font["default"], 20));
  Widget	*serverIp = new Widget("serverIp", {zone.left + 10, zone.top + 75,
					zone.width / 2, 25},
					sf::Text("Cancel", _font["default"], 20));
  Widget	*caButton = new Widget("ca", {zone.left + 10, zone.top + 110,
					zone.width / 2 - 17, 45},
					sf::Text("Cancel", _font["default"], 20));
  Widget	*coButton = new Widget("co", {zone.left + zone.width / 2 + 7,
					zone.top + 110, zone.width / 2 - 17, 45},
					sf::Text("Connect", _font["default"], 20));
  Widget	*addFav = new Widget("co", {zone.left + 10,
					zone.top + 166, zone.width / 2 - 17, 45},
					sf::Text("Fav", _font["default"], 20));
  Widget	*remFav = new Widget("co", {zone.left + zone.width / 2 + 7,
					zone.top + 166, zone.width / 2 - 17, 45},
					sf::Text("unFav", _font["default"], 20));

  popup->setState(APanelScreen::State::Leader);
  popup->setHide(true);
  addSpriteForWidget(bgWidget, sf::Color(0x11, 0x1E, 0x5E, 240), {zone.width - 2, zone.height});
  bgWidget->setEdge(std::unique_ptr<sf::RectangleShape>
		    (new sf::RectangleShape(sf::Vector2f(zone.width, zone.height))),
		    2.f);
  createServerPopupText(serverName);
  createServerPopupText(serverIp);
  createPopupHeader(header);
  createCancelButton(caButton, texture);
  createConnectButton(coButton, texture);
  createAddFavButton(serverIp->getTextContent(), addFav, texture);
  createRemFavButton(serverIp->getTextContent(), remFav, texture);
  remFav->addObserver(panels.at(1)); // fav Panel
  popup->addObserver({panels.at(0)}); // container
  setServerPopupTrigger(popup);
  caButton->addObserver(popup);
  coButton->addObserver({popup});
  popup->addWidget({bgWidget, serverName, serverIp, header, caButton, coButton, addFav, remFav});
  popup->construct(texture, set, {});
  addPanel({popup});
  return popup;
}

void	ServerMenu::setServerPopupTrigger(Panel *panel)
{
  std::function<void (const t_event &event)>  func;

  func = [panel](const t_event &event) // cannot call APanelScreen::trigger
    {
      if (event.e & wEvent::Hide)
	{
	  if (event.e & wEvent::Toggle)
	    panel->setHide(!panel->isHidden());
	  else
	    panel->setHide(true);
	}
      if (event.e & wEvent::UpdateText)
	{
	  std::string	ip;
	  std::string	servName;
	  std::size_t	pos;

	  pos = event.str.find("\n");
	  if (pos == std::string::npos)
	    throw std::runtime_error("Missing arguments");
	  panel->getWidget("serverName")->setTextContent(event.str.substr(0, pos));
	  panel->getWidget("serverIp")->setTextContent(event.str.substr(pos + 1));
	  //treatment here
	}
      if (event.e & wEvent::Update && event.e & wEvent::Hide) // means the connect button is clicked
	{
	  t_event evt;

	  evt.e = wEvent::Update;
	  evt.str = panel->getWidget("serverIp")->getTextContent();
	  panel->notify(evt);
	}
      if (event.e & wEvent::Reset || event.e & wEvent::Hide)
	{
	  t_event	evt = event;
	  const std::vector<AWidget *>	&widgets = panel->getWidgets();

	  evt.e = static_cast<wEvent>(evt.e & wEvent::None) | wEvent::Reset;
	  for (AWidget *widget : widgets)
	    widget->trigger(evt);
	}
    };
  panel->setTrigger(func);
}

void	ServerMenu::createAddFavButton(const sf::String &ip, Widget *widget,
				       const sf::Texture &texture)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [&](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(event, sf::Mouse::Left))
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

  std::function<void (const t_event &event)>  tfunc;

  tfunc = [widget](const t_event &event) // cannot call APanelScreen::trigger
    {
      if (event.e & wEvent::Reset)
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
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [&](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(event, sf::Mouse::Left))
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

  std::function<void (const t_event &event)>  tfunc;

  tfunc = [widget](const t_event &event) // cannot call APanelScreen::trigger
    {
      if (event.e & wEvent::Reset)
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
  Panel	*popup = new Panel(sf::FloatRect{_zone.left + _zone.width / 2 - 150,
	_zone.top + _zone.height / 2 - 85, 300, 170});
  sf::FloatRect	zone = popup->getZone();
  Widget	*bgWidget = new Widget("bg", {zone.left + 2, zone.top,
				zone.width - 4, zone.height - 2}, sf::Text());
  Widget	*header = new Widget("header", {zone.left, zone.top,
					zone.width, 40},
					sf::Text("Connect to Ip", _font["default"], 20));
  TextWidget	*input = new TextWidget("ip", {zone.left + 10, zone.top + 57,
					zone.width - 20, 35},
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
  addSpriteForWidget(bgWidget, sf::Color(0x11, 0x1E, 0x5E, 240), {zone.width - 4, zone.height});
  bgWidget->setEdge(std::unique_ptr<sf::RectangleShape>
		    (new sf::RectangleShape(sf::Vector2f(zone.width, zone.height))),
		    2.f);
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
  std::function<void (const t_event &event)>  func;

  func = [panel](const t_event &event) // cannot call APanelScreen::trigger
    {
      if (event.e & wEvent::Hide)
	{
	  if (event.e & wEvent::Toggle)
	    panel->setHide(!panel->isHidden());
	  else
	    panel->setHide(true);
	}
      if (event.e & wEvent::Update)
	{
	  const std::vector<AWidget *> &widgets = panel->getWidgets();
	  t_event	evt = event;
	  std::string	ip;

	  for (AWidget *widget : widgets)
	    if (widget->getFlag() & wFlag::Getable)
	      ip = widget->getContent();
	  evt.str = ip;
	  panel->notify(evt);
	}
      if (event.e & wEvent::Reset)
	{
	  t_event	evt = event;
	  const std::vector<AWidget *>	&widgets = panel->getWidgets();

	  evt.e = static_cast<wEvent>(evt.e & wEvent::None) | wEvent::Reset;
	  for (AWidget *widget : widgets)
	    widget->trigger(evt);
	}
    };
  panel->setTrigger(func);
}

void	ServerMenu::createServerPopupText(Widget *widget)
{
  sf::FloatRect zone = widget->getZone();

  widget->alignTextLeft({zone.left, zone.top}, {zone.width, zone.height}, 0, 50);
}

void	ServerMenu::createTitle(Widget *title)
{
  sf::FloatRect zone = title->getZone();

  addSpriteForWidget(title, sf::Color(50, 30, 60, 255), {zone.width, zone.height});
  title->alignText({zone.left, zone.top}, {zone.width, 100}, 50, 50);
}

void	ServerMenu::createPopupHeader(Widget *widget)
{
  sf::FloatRect zone = widget->getZone();

  addSpriteForWidget(widget, sf::Color(0x91, 0x4D, 0x03, 255), {zone.width, zone.height});
  widget->alignText({zone.left, zone.top}, {zone.width, zone.height}, 50, 50);
  widget->setEdge(std::unique_ptr<sf::RectangleShape>
	  (new sf::RectangleShape(sf::Vector2f(zone.width, zone.height))),
	  2.f);
}

void	ServerMenu::createFooter(Widget *footer)
{
  sf::FloatRect zone = footer->getZone();

  addSpriteForWidget(footer, sf::Color(50, 30, 60, 255), {zone.width, zone.height});
  footer->alignText({zone.left, zone.top}, {zone.width, zone.height}, 50, 50);
}

void	ServerMenu::createTabButton(TabWidget *widget)
{
  sf::FloatRect	zone = widget->getZone();

  addSpriteForWidget(widget, sf::Color(100, 100, 100, 100), {zone.width, zone.height});
  addSpriteForWidget(widget, sf::Color(100, 100, 100, 200), {zone.width, zone.height});
  widget->setSpriteAttr(1, false);
  widget->alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
}

void	ServerMenu::createButtonStyle(Widget *widget, const sf::Texture &texture)
{
  sf::FloatRect zone = widget->getZone();

  widget->alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
  widget->addSprite(texture, sf::IntRect(5, 1085, 250, 50));
  widget->addSprite(texture, sf::IntRect(265, 1085, 250, 50), false);
  widget->setSpriteSize(0, zone.width, zone.height);
  widget->setSpriteSize(1, zone.width, zone.height);
  widget->setEdge(std::unique_ptr<sf::RectangleShape>
		  (new sf::RectangleShape(sf::Vector2f(zone.width, zone.height))),
		  2.f);
}

void	ServerMenu::createButtonBack(Widget *widget, const sf::Texture &texture)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(event, sf::Mouse::Left))
	    {
	      lwidget.notify(t_event(wEvent::Hide | wEvent::Toggle));
	      return 0;
	    }
	}
      return 0;
    };
  createButtonStyle(widget, texture);
  widget->setUpdate(updateFunc);
}

void	ServerMenu::createButtonCip(Widget *widget, const sf::Texture &texture)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(event, sf::Mouse::Left))
	    {
	      lwidget.notify(t_event(wEvent::Hide | wEvent::Toggle));
	      return 1;
	    }
	}
      return 0;
    };
  createButtonStyle(widget, texture);
  widget->setUpdate(updateFunc);
}

void	ServerMenu::createButtonJoin(Widget *widget, const sf::Texture &texture)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(event, sf::Mouse::Left))
	    {
	      lwidget.notify(t_event(wEvent::Hide | wEvent::Toggle));
	      return 0;
	    }
	}
      return 0;
    };
  // set a trigger so everyone can observ him and send him the ip
  createButtonStyle(widget, texture);
  widget->setUpdate(updateFunc);
}

void	ServerMenu::createTextWidget(TextWidget *wTextWidget, const sf::Texture &texture)
{
  sf::FloatRect	zone = wTextWidget->getZone();
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &widget, const sf::Event &event UNUSED, sf::RenderWindow &ref UNUSED)
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
  wTextWidget->setEdge(std::unique_ptr<sf::RectangleShape>
		       (new sf::RectangleShape(sf::Vector2f(zone.width, zone.height))),
		       2.f);
}

void	ServerMenu::createCancelButton(Widget *widget, const sf::Texture &texture)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(event, sf::Mouse::Left))
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
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(event, sf::Mouse::Left))
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

  widget->addSprite(texture, sf::IntRect(1012, 1085, 13, 13));
  widget->addSprite(texture, sf::IntRect(1025, 1085, 13, 13));
  widget->addSprite(texture, sf::IntRect(1038, 1085, 13, 13));
  widget->toSize(0, 13, zone.height);
}
