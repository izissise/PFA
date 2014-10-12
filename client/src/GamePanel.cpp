#include <iostream>
#include "GamePanel.hpp"
#include "SimplexNoise.h"

GamePanel::GamePanel(const sf::FloatRect &zone) :
  APanelScreen(zone), _pad(0), _padup(0), _oldY(SHEIGHT), _dir(true)
{
  setHide(true);
}

GamePanel::~GamePanel()
{
}

void	GamePanel::construct(const sf::Texture &texture UNUSED, Settings &set UNUSED,
			     const std::vector<APanelScreen *> &panels UNUSED)
{
  _sprites[0] = sf::Sprite(texture, sf::IntRect(1051, 1085, 15, 15));
  _sprites[1] = sf::Sprite(texture, sf::IntRect(1066, 1085, 15, 15));
  constructLine();
}

void	GamePanel::drawHud(sf::RenderWindow &window, bool toWin)
{
  APanelScreen::draw(window, toWin);
}

void		GamePanel::fillVertex(sf::Vector2f &prev, sf::Vector2f &next, int x)
{
  int		points = pow(2, ITERATIONS);
  int		pos = x / (1600.f / points);

  prev = sf::Vector2f(_line.getPoint(pos).position.x, _line.getPoint(pos).position.y);
  pos += 1;
  next = sf::Vector2f(_line.getPoint(pos).position.x, _line.getPoint(pos).position.y);
}

void		GamePanel::completeField(sf::RenderWindow &window)
{
  sf::Vector2f	prev;
  sf::Vector2f	next;
  float		a;
  float		b;

  for (int y = SHEIGHT + MHEIGHT; y > 0; y -= 15)
    {
      for (int x = 0; x < 1600; x += 15)
	{
	  fillVertex(prev, next, x);
	  if (y > prev.y && y > next.y)
	    {
	      _sprites[0].setPosition(x, y);
	      window.draw(_sprites[0]);
	    }
	  else
	    {
	      a = (next.y - prev.y) / (next.x - prev.x);
	      b = next.y - a * next.x;
	      if (y > a * x + b)
	      	{
	      	  _sprites[0].setPosition(x, y);
	      	  window.draw(_sprites[0]);
	      	}
	    }
	}
    }
}

void	GamePanel::draw(sf::RenderWindow &window, bool toWin)
{
  float	p;
  int	uplimit = (_padup == 0) ? SHEIGHT + MHEIGHT :
    (_padup < 0) ? 900 : 0;

  for (int y = 900; y >= uplimit; y -= 15)
    {
      for (int x = 0; x < 1600; x += 15)
	{
	  p = octave_noise_2d(2, 1, 0.05,
			      (static_cast<float>(x) +
			       static_cast<float>(_pad)) / 15.f,
			      (static_cast<float>(y) +
			       static_cast<float>(_padup)) / 15.f);
	  if (p >= 0)
	    {
	      if (p < 0.1)
		{
		  _sprites[1].setPosition(x, y);
		  window.draw(_sprites[1]);
		}
	      else
		{
		  _sprites[0].setPosition(x, y);
		  window.draw(_sprites[0]);
		}
	    }
	}
    }
  if (_padup <= 0)
    {
      completeField(window);
      _line.draw(window);
    }
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
      _line.points.push_back(sf::Vertex(sf::Vector2f(0, _dir ? _oldY :
						     SHEIGHT + static_cast<float>(MHEIGHT) *
						     raw_noise_2d(_pad, _oldY))));
      _line.points.push_back(sf::Vertex(sf::Vector2f(1600, _dir ? SHEIGHT +
						     static_cast<float>(MHEIGHT) *
						     raw_noise_2d(_pad + 1600, _oldY) : _oldY)));
    }
  for (int i = 0; i < ITERATIONS; ++i)
    {
      for (int j = 0; j < cutPoints; ++j)
	{
	  beg = _line.points.begin();
	  prev = _line.getPointFromList(j * 2);
	  next = _line.getPointFromList(j * 2 + 1);
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

  if (event.type == sf::Event::KeyPressed &&
      (event.key.code == sf::Keyboard::Right ||
       event.key.code == sf::Keyboard::Left))
    {
      if ((_dir = ((event.key.code == sf::Keyboard::Right))))
	_pad += 1600;
      else
	_pad -= 1600;
      _oldY = _dir ? _line.getPoint(_line.size() - 1).position.y :
	_line.getPoint(0).position.y;
      _line.clear();
      constructLine();
    }
  else if (event.type == sf::Event::KeyPressed &&
	   (event.key.code == sf::Keyboard::Down ||
	    event.key.code == sf::Keyboard::Up))
    {
      if (event.key.code == sf::Keyboard::Down)
	_padup += 900;
      else
	_padup -= 900;
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
