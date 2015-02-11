#include <iostream>
#include "MainMenu.hpp"

MainMenu::MainMenu(const sf::FloatRect &zone) :
  APanelScreen(zone)
{
  addFont("default", "../client/assets/default.TTF");
}

MainMenu::~MainMenu()
{
  for (auto &widget : _widgets)
    delete widget;
}

void	MainMenu::construct(const sf::Texture &texture, Settings &set,
			    const std::vector<APanelScreen *> &panels)
{
  Widget	*wBackground = new Widget("background", {0, 0, SIZEX, SIZEY});
  Widget	*wMback = new Widget("menuback", {30, 100, 300, 600});
  Widget	*wMenuTitle = new Widget("menu-title", {30, 100, 300, 70},
  					 sf::Text("MAIN", _font["default"], 40));
  Widget	*wPlay = new Widget("play", {50, 200, 260, 60},
  				    sf::Text("PLAY ONLINE", _font["default"], 30));
  Widget	*wOpt = new Widget("opt", {50, 280, 260, 60},
				   sf::Text("OPTION", _font["default"], 30));
  Widget	*wQuit = new Widget("quit", {50, 360, 260, 60},
				    sf::Text("QUIT", _font["default"], 30));

  createBgPanel(texture, wBackground, wMback, wMenuTitle);
  createPlayButton(texture, wPlay);
  createOptButton(texture, wOpt);
  createQuitButton(texture, wQuit);

  wPlay->addObserver(this);
  wPlay->addObserver(panels[1]);
  wOpt->addObserver(panels[0]);

  _widgets.push_back(wBackground);
  _widgets.push_back(wMback);
  _widgets.push_back(wMenuTitle);
  _widgets.push_back(wPlay);
  _widgets.push_back(wOpt);
  _widgets.push_back(wQuit);
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	MainMenu::createBgPanel(const sf::Texture &texture, Widget *wBackground,
				Widget *wMback, Widget *wMenuTitle)
{
  sf::FloatRect zone = wMenuTitle->getZone();

  wBackground->addSprite(texture, sf::IntRect(0, 0, 1920, 1080));
  addSpriteForWidget(wMback, sf::Color(125, 125, 125, 150), {300, 600});
  addSpriteForWidget(wMenuTitle, sf::Color(10, 06, 12, 255), {300, 70});
  wMenuTitle->setTextAttr(sf::Text::Bold);
  wMenuTitle->alignText({zone.left,zone.top}, {zone.width, zone.height}, 25, 50);
}

void	MainMenu::createPlayButton(const sf::Texture &texture, Widget *wPlay)
{
  sf::FloatRect zone = wPlay->getZone();
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = widget.isOver(ref);
      widget.setSpriteAttr(0, !isOver);
      widget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (widget.isClicked(ev, sf::Mouse::Left))
	    {
	      widget.notify(t_event(wEvent::Hide | wEvent::Toggle));
	      return 0;
	    }
	}
      return 0;
    };
  wPlay->alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
  wPlay->addSprite(texture, sf::IntRect(0, 1080, zone.width, zone.height));
  wPlay->addSprite(texture, sf::IntRect(260, 1080, zone.width, zone.height), false);
  wPlay->setUpdate(updateFunc);
}

void	MainMenu::createOptButton(const sf::Texture &texture, Widget *wOpt)
{
  sf::FloatRect zone = wOpt->getZone();
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = widget.isOver(ref);
      widget.setSpriteAttr(0, !isOver);
      widget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (widget.isClicked(ev, sf::Mouse::Left))
	    {
	      widget.notify(t_event(wEvent::Hide | wEvent::Toggle, 0, 0));
	      return 1;
	    }
	}
      return 0;
    };
  wOpt->alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
  wOpt->addSprite(texture, sf::IntRect(0, 1080, zone.width, zone.height));
  wOpt->addSprite(texture, sf::IntRect(260, 1080, zone.width, zone.height), false);
  wOpt->setUpdate(updateFunc);
}

void	MainMenu::createQuitButton(const sf::Texture &texture, Widget *wQuit)
{
  sf::FloatRect zone = wQuit->getZone();
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = widget.isOver(ref);
      widget.setSpriteAttr(0, !isOver);
      widget.setSpriteAttr(1, isOver);
      if (isOver)
	{
 	 if (widget.isClicked(ev, sf::Mouse::Left))
	   {
	     ref.close();
	     return 1;
	   }
	}
      return 0;
    };
  wQuit->alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
  wQuit->addSprite(texture, sf::IntRect(0, 1080, zone.width, zone.height));
  wQuit->addSprite(texture, sf::IntRect(260, 1080, zone.width, zone.height), false);
  wQuit->setUpdate(updateFunc);
}
