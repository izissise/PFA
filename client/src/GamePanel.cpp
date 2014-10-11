#include <iostream>
#include "GamePanel.hpp"
#include "SimplexNoise.h"

GamePanel::GamePanel(const sf::FloatRect &zone) :
  APanelScreen(zone)
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
  std::list<sf::Vertex>::const_iterator	beg;
  sf::Vertex prev;
  sf::Vertex next;
  int	cutPoints;
  int	height;
  int	x;
  int	y;

  if (_line.points.empty())
    {
      _line.points.push_back(sf::Vertex(sf::Vector2f(0, 500)));
      _line.points.push_back(sf::Vertex(sf::Vector2f(1600, 500)));
    }
  for (int i = 0; i < ITERATIONS; ++i)
    {
      cutPoints = fastPow(2, i);
      for (int j = 0; j < cutPoints; ++j)
	{
	  beg = _line.points.cbegin();
	  prev = _line.getPoint(j * 2);
	  next = _line.getPoint(j * 2 + 1);
	  x = next.position.x / 2 + prev.position.x / 2;
	  y = next.position.y / 2 + prev.position.y / 2;
	  height = (static_cast<float>(MHEIGHT) /
		    (static_cast<float>(i) * 2.f + 1.f) ) * raw_noise_2d(x + 0, y);
	  std::advance(beg, j * 2 + 1);
	  _line.points.insert(beg, sf::Vertex(sf::Vector2f(x, y + height)));
	}
    }
  _line.update();
}

int	GamePanel::fastPow(int x, int it) const
{
  int	res = 1;

  while (it > 0)
    {
      res = res * x;
      --it;
    }
  return res;
}
