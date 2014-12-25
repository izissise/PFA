#include "ServerMenu.hpp"
#include "ServerInfoPanel.hpp"

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
  Panel *serv = createServListPanel(set, texture, panels);
  Panel *fav = createFavPanel(set, texture, panels);
  Panel *cont = createContPanel(set, texture, {serv, fav});
  Panel *popup = createCoPopup(set, texture, panels);
  Panel	*serverPopup = createServerPopup(set, texture, {});
  wConnectIp->addObserver(popup);
  createTabBar(set, texture, {serv, fav});
  createFooter(wFooter);
  createButtonBack(wBack, texture);
  createButtonCip(wConnectIp, texture);
  createButtonJoin(wJoin, texture);
  wBack->addObserver({this, panels[0]});
  wJoin->addObserver({this, panels[1]});


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

  content->setDisplayFlag(APanelScreen::Display::Overlap);
  triggerFunc = [this](const t_event &event)
    {
      if (event.e & wEvent::Update) // Means a connect to ip
	{
	  APanelScreen	*shownPanel = nullptr; // Should not be null after the loop

	  for (APanelScreen *pan : _panels)
	    if (pan->isHidden() == false)
	      {
		shownPanel = pan;
		break ;
	      }
	}
    };
  content->setTrigger(triggerFunc);
  content->addPanel({panels[0], panels[1]});
  content->construct(texture, set, {});
  addPanel(content);
  return content;
}

Panel	*ServerMenu::createServListPanel(Settings &set, const sf::Texture &texture,
					 const std::vector<APanelScreen *> &panels)
{
  Panel	*content = new Panel(sf::FloatRect{_zone.left, _zone.top + 140,
	_zone.width, _zone.height - 210});
  sf::FloatRect	zone = content->getZone();
  Widget	*bgWidget = new Widget("bg", {zone.left, zone.top,
				zone.width, zone.height}, sf::Text());
  ScrollWidget	*wScroll = new ScrollWidget("scroll",
					    {zone.left + zone.width - 13, zone.top, 13, zone.height},
					    Scroll::Vertical, content,
					    sf::Text(), wFlag::None);
  Panel		*capsule = encapsulate(wScroll);

  createScrollBar(wScroll, texture);
  capsule->setState(APanelScreen::State::Static);
  capsule->construct(texture, set, {});

  addSpriteForWidget(bgWidget, sf::Color(200, 200, 200, 255), {zone.width, zone.height});
  content->addWidget({bgWidget});
  for (unsigned int i = 0; i < 50; ++i)
    addServerToList(content, set, texture, "127.0.0.1");
  content->addPanel({capsule});
  content->construct(texture, set, {});
  return content;
}

Panel	*ServerMenu::createFavPanel(Settings &set, const sf::Texture &texture,
				   const std::vector<APanelScreen *> &panels)
{
  Panel	*content = new Panel(sf::FloatRect{_zone.left, _zone.top + 140,
	_zone.width, _zone.height - 210});
  sf::FloatRect	zone = content->getZone();
  Widget	*bgWidget = new Widget("bg", {zone.left, zone.top,
				zone.width, zone.height}, sf::Text());
  ScrollWidget	*wScroll = new ScrollWidget("scroll",
					    {zone.left + zone.width - 13, zone.top, 13, zone.height},
					    Scroll::Vertical, content,
					    sf::Text(), wFlag::None);
  Panel		*capsule = encapsulate(wScroll);

  createScrollBar(wScroll, texture);
  capsule->setState(APanelScreen::State::Static);
  capsule->construct(texture, set, {});

  addSpriteForWidget(bgWidget, sf::Color(200, 200, 200, 255), {zone.width, zone.height});
  content->addWidget({bgWidget});
  content->addPanel({capsule});
  content->construct(texture, set, {});
  content->setHide(true);
  return content;
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
  unsigned int		nbElem = 0;

  for (APanelScreen *pan : panels[0]->getSubPanels())
    ++nbElem;
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateDisplay =
    [](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      return 0;
    };

  wBg->setUpdate(updateDisplay);
  if (nbElem % 2)
    addSpriteForWidget(wBg, sf::Color(100, 100, 100, 255), {zone.width, zone.height});
  else
    addSpriteForWidget(wBg, sf::Color(130, 130, 130, 255), {zone.width, zone.height});
  addSpriteForWidget(wBg, sf::Color(0x40, 0x6F, 0x39, 255), {zone.width, zone.height});
  wBg->setSpriteAttr(1, false);

  style->addWidget(wBg);
  style->construct(texture, set, {});
  serverInfoPanel->construct(texture, set, {});
  controler->addPanel({style, serverInfoPanel});
  controler->construct(texture, set, {});
  return controler;
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

void	ServerMenu::addServerToList(APanelScreen *list,
				    Settings &set,
				    const sf::Texture &texture,
				    const std::string &ip)
{
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
  list->addPanel(pan);
}

Panel	*ServerMenu::createServerPopup(Settings &set, const sf::Texture &texture,
				       const std::vector<APanelScreen *> &panels)
{
  Panel	*popup = new Panel(sf::FloatRect{_zone.left + _zone.width / 2 - 200,
	_zone.top + _zone.height / 2 - 110, 400, 220});
  sf::FloatRect	zone = popup->getZone();
  Widget	*bgWidget = new Widget("bg", {zone.left + 2, zone.top,
				zone.width - 4, zone.height - 2}, sf::Text());
  Widget	*header = new Widget("header", {zone.left, zone.top,
					zone.width, 40},
					sf::Text("Server's Options", _font["default"], 20));

  Widget	*serverName = new Widget("", {zone.left + 10, zone.top + 10,
				zone.width / 2, 25},
				sf::Text("Server Name", _font["default"], 20));
  Widget	*serverIp = new Widget("", {zone.left + 10, zone.top + 10,
					zone.width / 2, 25},
					sf::Text("Cancel", _font["default"], 20));
  Widget	*caButton = new Widget("ca", {zone.left + 10, zone.top + 110,
					zone.width / 2 - 17, 45},
					sf::Text("Cancel", _font["default"], 20));
  Widget	*coButton = new Widget("co", {zone.left + zone.width / 2 + 7,
					zone.top + 110, zone.width / 2 - 17, 45},
					sf::Text("Connect", _font["default"], 20));
  Widget	*addFav = new Widget("co", {zone.left + 10,
					zone.top + 150, zone.width / 2 - 17, 45},
					sf::Text("Add to Favorite", _font["default"], 20));
  Widget	*remFav = new Widget("co", {zone.left + zone.width / 2 + 7,
					zone.top + 150, zone.width / 2 - 17, 45},
					sf::Text("Remove From Favorite", _font["default"], 20));

  popup->setState(APanelScreen::State::Leader);
  // popup->setHide(true);
  addSpriteForWidget(bgWidget, sf::Color(100, 100, 100, 150), {zone.width, zone.height});
  createPopupHeader(header);
  createCancelButton(caButton, texture);
  createConnectButton(coButton, texture);
  createCancelButton(addFav, texture);
  createCancelButton(remFav, texture);
  //  popup->addObserver({panels[1], this}); // gamePanel
  setServerPopupTrigger(popup);
  caButton->addObserver(popup);
  coButton->addObserver({popup});
  popup->addWidget({bgWidget, header, caButton, coButton, addFav, remFav});
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
	  const std::vector<AWidget *> &widgets = panel->getWidgets();
	  //treatment here
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
  TextWidget	*input = new TextWidget("ip", {zone.left + 10, zone.top + 50,
					zone.width - 20, 45},
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
  addSpriteForWidget(bgWidget, sf::Color(100, 100, 100, 150), {zone.width, zone.height});
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

void	ServerMenu::createTitle(Widget *title)
{
  sf::FloatRect zone = title->getZone();

  addSpriteForWidget(title, sf::Color(50, 30, 60, 255), {zone.width, zone.height});
  title->alignText({zone.left, zone.top}, {zone.width, 100}, 50, 50);
}

void	ServerMenu::createPopupHeader(Widget *widget)
{
  sf::FloatRect zone = widget->getZone();

  addSpriteForWidget(widget, sf::Color(50, 30, 60, 255), {zone.width, zone.height});
  widget->alignText({zone.left, zone.top}, {zone.width, zone.height}, 50, 50);
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
	      return 0;
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
