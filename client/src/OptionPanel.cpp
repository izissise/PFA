#include "OptionPanel.hpp"

OptionPanel::OptionPanel() :
  APanelScreen()
{
  if (!_font.loadFromFile("../client/assets/font.otf"))
    std::cerr << "Can't load font" << std::endl; // replace this by a throw about ressources
  _hide = true;
}

OptionPanel::~OptionPanel()
{
}

void	OptionPanel::construct(const sf::Texture &texture, Settings &set,
			       const std::vector<APanelScreen *> &panels)
{
  Widget	*wPanOption = new Widget("panOpt", {380, 120, 1100, 730});
  Widget	*wBarGame = new Widget("gameBar", {380, 50, 260, 70}, sf::Text("Game", _font, 30));
  Widget	*wBarKeyboard = new Widget("keyboardBar", {640, 50, 260, 70},
					   sf::Text("Keyboard", _font, 30));

  createOptPanel(texture, wPanOption);
  createBarGame(texture, wBarGame);
  createBarKeyboard(texture, wBarKeyboard);

  wBarGame->addObserver(wBarKeyboard);
  wBarKeyboard->addObserver(wBarGame);

  _widgets.push_back(wPanOption);
  _widgets.push_back(wBarGame);
  _widgets.push_back(wBarKeyboard);
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	OptionPanel::createOptPanel(const sf::Texture &texture, Widget *wPanOption)
{
  addSpriteForWidget(wPanOption, sf::Color(125, 125, 125, 150), {1100, 730});
}

void	OptionPanel::createBarGame(const sf::Texture &texture, Widget *wBarGame)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = widget.isOver(ref);
      if (isOver)
	{
 	 if (widget.isClicked(event, sf::Mouse::Left))
	   {
	     widget.notify(t_event(wEvent::SetSprite, 0));
	     widget.setSprite(1);
	     return 1;
	   }
	}
      return 0;
    };
  addSpriteForWidget(wBarGame, sf::Color(65, 65, 65, 150), {260, 70});
  addSpriteForWidget(wBarGame, sf::Color(125, 125, 125, 150), {260, 70});
  wBarGame->setSprite(1);
  wBarGame->alignText({380,50}, {260, 70}, 50, 50);
  wBarGame->setFunction("main", updateFunc);
}

void	OptionPanel::createBarKeyboard(const sf::Texture &texture, Widget *wBarKeyboard)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = widget.isOver(ref);
      if (isOver)
	{
 	 if (widget.isClicked(event, sf::Mouse::Left))
	   {
	     widget.notify(t_event(wEvent::SetSprite, 0));
	     widget.setSprite(1);
	     return 1;
	   }
	}
      return 0;
    };
  addSpriteForWidget(wBarKeyboard, sf::Color(65, 65, 65, 150), {260, 70});
  addSpriteForWidget(wBarKeyboard, sf::Color(125, 125, 125, 150), {260, 70});
  wBarKeyboard->alignText({640,50}, {260, 70}, 50, 50);
  wBarKeyboard->setFunction("main", updateFunc);
}
