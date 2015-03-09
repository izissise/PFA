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
					  sf::Text("Keyboard", _font["default"], 30), panels[1]);
  TabWidget	*wBarKeyboard = new TabWidget("keyboardBar", {640, 50, 260, _zone.height},
					      sf::Text("Game", _font["default"], 30), panels[0]);

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
  addSpriteForWidget(wBarGame, sf::Color(0, 0, 0, 100), {260, _zone.height}, false);
  addSpriteForWidget(wBarGame, sf::Color(125, 125, 125, 100), {260, _zone.height}, true);
  wBarGame->alignText({380,50}, {260, _zone.height}, 50, 50);
}

void	OptionTabPanel::createBarKeyboard(const sf::Texture &texture UNUSED, TabWidget *wBarKeyboard)
{
  addSpriteForWidget(wBarKeyboard, sf::Color(0, 0, 0, 100), {260, _zone.height});
  addSpriteForWidget(wBarKeyboard, sf::Color(125, 125, 125, 100), {260, _zone.height}, false);
  wBarKeyboard->alignText({640,50}, {260, _zone.height}, 50, 50);
}
