#include "OptionPanel.hpp"
#include "Unused.hpp"

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

void	OptionPanel::construct(const sf::Texture &texture UNUSED, Settings &set,
			       const std::vector<APanelScreen *> &panels UNUSED)
{
  Widget	*wPanOption = new Widget("panOpt", {380, 120, 1100, 730});
  Widget	*wBarGame = new Widget("gameBar", {380, 50, 260, 70}, sf::Text("Game", _font, 30));
  Widget	*wBarKeyboard = new Widget("keyboardBar", {640, 50, 260, 70},
					   sf::Text("Keyboard", _font, 30));
  Widget	*wCheckBox = new Widget("checkBox", {450, 200, 50, 50});

  createOptPanel(texture, wPanOption);
  createBarGame(texture, wBarGame);
  createBarKeyboard(texture, wBarKeyboard);
  createCheckBox(texture, wCheckBox);

  wBarGame->addObserver(wBarKeyboard);
  wBarKeyboard->addObserver(wBarGame);

  _widgets.push_back(wPanOption);
  _widgets.push_back(wBarGame);
  _widgets.push_back(wBarKeyboard);
  _widgets.push_back(wCheckBox);
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	OptionPanel::createOptPanel(const sf::Texture &texture UNUSED, Widget *wPanOption)
{
  addSpriteForWidget(wPanOption, sf::Color(125, 125, 125, 150), {1100, 730});
}

void	OptionPanel::createBarGame(const sf::Texture &texture UNUSED, Widget *wBarGame)
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
	     widget.notify(t_event(wEvent::SetSprite, 0, 1));
	     widget.notify(t_event(wEvent::SetSprite, 1, 0));
	     widget.setSpriteAttr(0, 0);
	     widget.setSpriteAttr(1, 1);
	     return 1;
	   }
	}
      return 0;
    };
  addSpriteForWidget(wBarGame, sf::Color(55, 55, 55, 150), {260, 70}, false);
  addSpriteForWidget(wBarGame, sf::Color(125, 125, 125, 150), {260, 70}, true);
  wBarGame->alignText({380,50}, {260, 70}, 50, 50);
  wBarGame->setFunction("main", updateFunc);
}

void	OptionPanel::createBarKeyboard(const sf::Texture &texture UNUSED, Widget *wBarKeyboard)
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
	     widget.notify(t_event(wEvent::SetSprite, 0, 1));
	     widget.notify(t_event(wEvent::SetSprite, 1, 0));
	     widget.setSpriteAttr(0, 0);
	     widget.setSpriteAttr(1, 1);
	     return 1;
	   }
	}
      return 0;
    };
  addSpriteForWidget(wBarKeyboard, sf::Color(55, 55, 55, 150), {260, 70});
  addSpriteForWidget(wBarKeyboard, sf::Color(125, 125, 125, 150), {260, 70}, false);
  wBarKeyboard->alignText({640,50}, {260, 70}, 50, 50);
  wBarKeyboard->setFunction("main", updateFunc);
}

void	OptionPanel::createCheckBox(const sf::Texture &texture, Widget *wCheckBox)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = widget.isOver(ref);
      widget.setSpriteAttr(0, !isOver);
      widget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (widget.isClicked(event, sf::Mouse::Left))
	    {
	      widget.toggleSpriteAttr(2);
	      return 1;
	    }
	}
      return 0;
    };
  wCheckBox->addSprite(texture, sf::IntRect(520, 1080, 50, 50));
  wCheckBox->addSprite(texture, sf::IntRect(570, 1080, 50, 50), false);
  wCheckBox->addSprite(texture, sf::IntRect(620, 1080, 32, 32), false);
  wCheckBox->setSpriteSize(2, 50, 50);
  wCheckBox->setFunction("main", updateFunc);
  wCheckBox->resize(0.5, 0.5);
}
