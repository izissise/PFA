#include <iostream>
#include "GamePanel.hpp"
#include "SimplexNoise.h"

GamePanel::GamePanel(const sf::FloatRect &zone) :
  APanelScreen(zone), _pad(0)
{
}

GamePanel::~GamePanel()
{
}

void	GamePanel::construct(const sf::Texture &texture UNUSED, Settings &set UNUSED,
			     const std::vector<APanelScreen *> &panels UNUSED)
{
  setHide(true);
  constructLine();
}

void	GamePanel::drawHud(sf::RenderWindow &window, bool toWin)
{
  APanelScreen::draw(window, toWin);
}

void	GamePanel::draw(sf::RenderWindow &window, bool toWin)
{
  _line.draw(window);
  drawHud(window, toWin);
}

void	GamePanel::constructLine()
{
  std::list<sf::Vertex>::iterator	beg;
  sf::Vertex prev;
  sf::Vertex next;
  int	cutPoints;
  int	height;
  int	x;
  int	y;

  cutPoints = 1;
  if (_line.points.empty())
    {
      _line.points.push_back(sf::Vertex(sf::Vector2f(0, 500)));
      _line.points.push_back(sf::Vertex(sf::Vector2f(1600, 500)));
    }
  for (int i = 0; i < ITERATIONS; ++i)
    {
      for (int j = 0; j < cutPoints; ++j)
	{
	  beg = _line.points.begin();
	  prev = _line.getPoint(j * 2);
	  next = _line.getPoint(j * 2 + 1);
	  x = next.position.x / 2 + prev.position.x / 2;
	  y = next.position.y / 2 + prev.position.y / 2;
	  height = (static_cast<float>(MHEIGHT) /
		    (static_cast<float>(i) * 2.f + 1.f)) * raw_noise_2d(x + _pad, y);
	  std::advance(beg, j * 2 + 1);
	  _line.points.insert(beg, sf::Vertex(sf::Vector2f(x, y + height)));
	}
      cutPoints *= 2;
    }
  _line.update();
}

int	GamePanel::update(const sf::Event &event, sf::RenderWindow &ref, Settings &set)
{
  int	retVal = 0;
  Controls &control = set.getControls();

  if (control.getActionState(Action::Right) ||
      control.getActionState(Action::Left))
    {
      if (control.getActionState(Action::Right))
	_pad += 1600;
      else
	_pad -= 1600;
      _line.clear();
      constructLine();
    }
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
  return retVal;
}
