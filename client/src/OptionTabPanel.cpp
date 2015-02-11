#include "OptionTabPanel.hpp"
#include "Unused.hpp"

OptionTabPanel::OptionTabPanel(const sf::FloatRect &zone) :
  APanelScreen(zone)
{
  addFont("default", "../client/assets/default.TTF");
  _hide = true;
}

OptionTabPanel::~OptionTabPanel()
{
}

void	OptionTabPanel::trigger(const t_event &ev)
{
  if (ev.e & wEvent::Hide)
    {
      if (ev.e & wEvent::Toggle)
	_hide = !_hide;
      else
	{
	  _hide = ev.value;
	}
    }
}

void	OptionTabPanel::construct(const sf::Texture &texture UNUSED, Settings &set,
			       const std::vector<APanelScreen *> &panels)
{
  TabWidget	*wBarGame = new TabWidget("gameBar", {380, 50, 260, _zone.height},
					  sf::Text("Game", _font["default"], 30), panels[0]);
  TabWidget	*wBarKeyboard = new TabWidget("keyboardBar", {640, 50, 260, _zone.height},
					      sf::Text("Keyboard", _font["default"], 30), panels[1]);

  createBarGame(texture, wBarGame);
  createBarKeyboard(texture, wBarKeyboard);

  wBarGame->addObserver(wBarKeyboard);
  wBarKeyboard->addObserver(wBarGame);

  _widgets.push_back(wBarGame);
  _widgets.push_back(wBarKeyboard);
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	OptionTabPanel::createBarGame(const sf::Texture &texture UNUSED, TabWidget *wBarGame)
{
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = widget.isOver(ref);
      if (isOver)
	{
 	 if (widget.isClicked(ev, sf::Mouse::Left))
	   {
	     widget.setSpriteAttr(0, 0);
	     widget.setSpriteAttr(1, 1);
	     dynamic_cast<TabWidget *>(&widget)->getPanel()->setHide(false);
	     widget.notify(t_event(wEvent::SetSprite, 0, 1));
	     widget.notify(t_event(wEvent::SetSprite, 1, 0));
	     return 1;
	   }
	}
      return 0;
    };
  addSpriteForWidget(wBarGame, sf::Color(0, 0, 0, 100), {260, _zone.height}, false);
  addSpriteForWidget(wBarGame, sf::Color(125, 125, 125, 100), {260, _zone.height}, true);
  wBarGame->alignText({380,50}, {260, _zone.height}, 50, 50);
  wBarGame->setUpdate(updateFunc);
}

void	OptionTabPanel::createBarKeyboard(const sf::Texture &texture UNUSED, TabWidget *wBarKeyboard)
{
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = widget.isOver(ref);
      if (isOver)
	{
 	 if (widget.isClicked(ev, sf::Mouse::Left))
	   {
	     widget.notify(t_event(wEvent::SetSprite, 0, 1));
	     widget.notify(t_event(wEvent::SetSprite, 1, 0));
	     dynamic_cast<TabWidget *>(&widget)->getPanel()->setHide(false);
	     widget.setSpriteAttr(0, 0);
	     widget.setSpriteAttr(1, 1);
	     return 1;
	   }
	}
      return 0;
    };
  addSpriteForWidget(wBarKeyboard, sf::Color(0, 0, 0, 100), {260, _zone.height});
  addSpriteForWidget(wBarKeyboard, sf::Color(125, 125, 125, 100), {260, _zone.height}, false);
  wBarKeyboard->alignText({640,50}, {260, _zone.height}, 50, 50);
  wBarKeyboard->setUpdate(updateFunc);
}
