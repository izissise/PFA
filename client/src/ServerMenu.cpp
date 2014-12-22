#include "ServerMenu.hpp"

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
  Widget	*wTitle = new Widget("title", {_zone.left, _zone.top, _zone.width, 70},
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
  createContPanel(set, texture, panels);
  createCoPopup(set, texture, panels, wConnectIp);
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

void	ServerMenu::createContPanel(Settings &set, const sf::Texture &texture,
				    const std::vector<APanelScreen *> &panels)
{
  Panel	*content = new Panel(sf::FloatRect{_zone.left, _zone.top + 70,
	_zone.width, _zone.height - 140});
  sf::FloatRect	contZone = content->getZone();
  Widget	*bgWidget = new Widget("bg", {contZone.left, contZone.top,
				contZone.width, contZone.height}, sf::Text());

  addSpriteForWidget(bgWidget, sf::Color(200, 200, 200, 255), {contZone.width, contZone.height});
  content->addWidget({bgWidget});
  content->construct(texture, set, {});
  addPanel({content});
}

void	ServerMenu::createCoPopup(Settings &set, const sf::Texture &texture,
				  const std::vector<APanelScreen *> &panels,
				  Widget *linkWidget)
{
  Panel	*popup = new Panel(sf::FloatRect{_zone.left + _zone.width / 2 - 150,
	_zone.top + _zone.height / 2 - 85, 300, 170});
  sf::FloatRect	popupZone = popup->getZone();
  Widget	*bgWidget = new Widget("bg", {popupZone.left + 2, popupZone.top,
				popupZone.width - 4, popupZone.height - 2}, sf::Text());
  Widget	*header = new Widget("header", {popupZone.left, popupZone.top,
					popupZone.width, 40},
					sf::Text("Connect to Ip", _font["default"], 20));
  TextWidget	*input = new TextWidget("ip", {popupZone.left + 10, popupZone.top + 50,
					popupZone.width - 20, 45},
					sf::Text("", _font["default"], 20),
					sf::Text("Ip", _font["default"], 20), 30);
  Widget	*caButton = new Widget("ca", {popupZone.left + 10, popupZone.top + 110,
					popupZone.width / 2 - 17, 45},
					sf::Text("Cancel", _font["default"], 20));
  Widget	*coButton = new Widget("co", {popupZone.left + popupZone.width / 2 + 7,
					popupZone.top + 110, popupZone.width / 2 - 17, 45},
					sf::Text("Connect", _font["default"], 20));

  popup->setState(APanelScreen::Leader);
  popup->setHide(true);
  addSpriteForWidget(bgWidget, sf::Color(100, 100, 100, 150), {popupZone.width, popupZone.height});
  bgWidget->setEdge(std::unique_ptr<sf::RectangleShape>
		    (new sf::RectangleShape(sf::Vector2f(popupZone.width, popupZone.height))),
		    2.f);
  createPopupHeader(header);
  createCancelButton(caButton, texture);
  createConnectButton(coButton, texture);
  createTextWidget(input, texture);
  popup->addObserver({panels[1], this}); // gamePanel
  setPopupTrigger(popup);
  linkWidget->addObserver(popup);
  caButton->addObserver(popup);
  coButton->addObserver({popup});
  popup->addWidget({bgWidget, header, input, caButton, coButton});
  popup->construct(texture, set, {});
  addPanel({popup});
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
  title->alignText({zone.left, zone.top}, {zone.width, zone.height}, 50, 50);
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
