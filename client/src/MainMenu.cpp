#include <iostream>
#include "MainMenu.hpp"

MainMenu::MainMenu(const sf::Texture &texture) :
  APanelScreen(texture)
{
  if (!_font.loadFromFile("../client/assets/font.otf"))
    std::cerr << "Can't load font" << std::endl; // replace this by a throw about ressources

  Widget	*wBackground = new Widget("background", {0, 0, 1920, 1080});
  Widget	*wMback = new Widget("menuback", {30, 50, 300, 600});
  Widget	*wMenuTitle = new Widget("menu-title", {30, 50, 300, 70},
  					 sf::Text("MAIN", _font, 40));
  Widget	*wPlay = new Widget("play", {50, 150, 260, 60},
  				    sf::Text("PLAY ONLINE", _font, 30));
  Widget	*wOpt = new Widget("opt", {50, 230, 260, 60}, sf::Text("OPTION", _font, 30));
  Widget	*wQuit = new Widget("quit", {50, 310, 260, 60}, sf::Text("QUIT", _font, 30));
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>	updateFunc;

  wBackground->addSprite(texture, sf::IntRect(0, 0, 1920, 1080));
  addSpriteForWidget(wMback, sf::Color(125, 125, 125, 150), {300, 600});
  addSpriteForWidget(wMenuTitle, sf::Color(10, 06, 12, 255), {300, 70});

  wMenuTitle->setTextAttr(sf::Text::Bold);
  wMenuTitle->alignText({30,50}, {300, 70}, 25, 50);

  updateFunc = [](AWidget &widget, const sf::Event &event, sf::RenderWindow &ref) -> int
    {
      bool	isOver;

      isOver = widget.isOver(ref);
      widget.eOver(isOver);
      if (isOver)
	{
	  if (widget.isClicked(event, sf::Mouse::Left))
	    {
	      return 1;
	    }
	}
      return 0;
    };
  wPlay->alignText({50,150}, {260, 60}, 50, 50);
  wPlay->addSprite(texture, sf::IntRect(0, 1080, 260, 60));
  wPlay->addSprite(texture, sf::IntRect(260, 1080, 260, 60));
  wPlay->setFunction("main", updateFunc);

  updateFunc = [](AWidget &widget, const sf::Event &event, sf::RenderWindow &ref) -> int
    {
      bool	isOver;

      isOver = widget.isOver(ref);
      widget.eOver(isOver);
      if (isOver)
	{
	  if (widget.isClicked(event, sf::Mouse::Left))
	    {
	      return 1;
	    }
	}
      return 0;
    };
  wOpt->alignText({50,230}, {260, 60}, 50, 50);
  wOpt->addSprite(texture, sf::IntRect(0, 1080, 260, 60));
  wOpt->addSprite(texture, sf::IntRect(260, 1080, 260, 60));
  wOpt->setFunction("main", updateFunc);

  updateFunc = [](AWidget &widget, const sf::Event &event, sf::RenderWindow &ref) -> int
    {
      bool	isOver;

      isOver = widget.isOver(ref);
      widget.eOver(isOver);
      if (isOver)
	{
 	 if (widget.isClicked(event, sf::Mouse::Left))
	   {
	     ref.close();
	     return 1;
	   }
	}
      return 0;
    };
  wQuit->alignText({50,310}, {260, 60}, 50, 50);
  wQuit->addSprite(texture, sf::IntRect(0, 1080, 260, 60));
  wQuit->addSprite(texture, sf::IntRect(260, 1080, 260, 60));
  wQuit->setFunction("main", updateFunc);

  _widgets.push_back(wBackground);
  _widgets.push_back(wMback);
  _widgets.push_back(wMenuTitle);
  _widgets.push_back(wPlay);
  _widgets.push_back(wOpt);
  _widgets.push_back(wQuit);
}

MainMenu::~MainMenu()
{
}

int	MainMenu::run(const sf::Event &event, sf::RenderWindow &ref, Settings &set)
{
  for (auto rit = _widgets.rbegin(); rit != _widgets.rend(); ++rit)
    (*rit)->update(event, ref, set);
}
