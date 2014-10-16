#include <iostream>
#include "GamePanel.hpp"
#include "SimplexNoise.h"

GamePanel::GamePanel(const sf::FloatRect &zone) :
  APanelScreen(zone), _pad(0), _padup(0), _oldY(SHEIGHT), _dir(true), _world(nullptr)
{
  setHide(true);
}

GamePanel::~GamePanel()
{
}

void	GamePanel::construct(const sf::Texture &texture UNUSED, Settings &set UNUSED,
			     const std::vector<APanelScreen *> &panels UNUSED)
{
  _world.reset(new World{set});
}

void	GamePanel::drawHud(sf::RenderWindow &window, bool toWin)
{
  APanelScreen::draw(window, toWin);
}

void	GamePanel::draw(sf::RenderWindow &window, bool toWin)
{
  _world->draw(window);
  drawHud(window, toWin);
}

int	GamePanel::update(const sf::Event &event, sf::RenderWindow &ref, Settings &set)
{
  int	retVal = 0;

  for (auto rit = _panels.rbegin(); rit != _panels.rend(); ++rit)
    {
      if ((*rit)->isHidden() == false)
	if ((retVal = (*rit)->update(event, ref, set)) != 0)
	  return retVal;
    }
  for (auto rit = _widgets.rbegin(); rit != _widgets.rend(); ++rit)
    {
      if ((retVal = (*rit)->update(event, ref, set)) != 0)
	return retVal;
    }
  _world->update();
  return retVal;
}
