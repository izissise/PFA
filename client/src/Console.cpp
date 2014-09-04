#include "Console.hpp"
#include "Exception.hpp"

Console::Console(Settings * const set) : _set(set), _history()
{
  sf::Color	color(20, 20, 20);
  int		width = std::stoi(set->getCvarList().getCvar("r_width"));
  int		height = std::stoi(set->getCvarList().getCvar("r_height"));

  _rectangle.setSize(sf::Vector2f(width, height / 2));
  _rectangle.setPosition(0, 0);
  _rectangle.setOutlineColor(sf::Color::White);
  _rectangle.setOutlineThickness(2);
  _rectangle.setFillColor(color);

  if (!_font.loadFromFile("../client/assets/font.otf"))
    std::cerr << "Can't load font" << std::endl; // replace this by a throw about ressources
  _text.setFont(_font);
  _text.setStyle(sf::Text::Regular);
  _text.setColor(sf::Color::White);
  _text.setCharacterSize(FONTSIZE);
}

Console::~Console()
{
}


void		Console::run(sf::RenderWindow &window, sf::Event event)
{
  Controls	&ctrl = _set->getControls();

  if (_input.getInput(event))
    {
      try {
	_set->parseCommandLine(_input.getString().toAnsiString());
      }
      catch (const Exception &e) {
	std::cerr << e.what() << std::endl;
      }
      _history.content.push_front(_input.getString());
      if (_history.content.size() > _history.maxSize)
	_history.content.pop_back();
      _input.clear();
    }
  if (event.type == sf::Event::KeyPressed)
    ctrl.pressKey(event.key.code);
  else if (event.type == sf::Event::KeyReleased)
    ctrl.releaseKey(event.key.code);
}

void		Console::draw(sf::RenderWindow &window)
{
  unsigned int	wSize = (FONTSIZE + 2 * LINESPACING);
  unsigned int	cSize;

  cSize = std::stoi(_set->getCvarList().getCvar("r_height")) / 2 - wSize;
  window.draw(_rectangle);
  _text.setPosition(0, cSize - (FONTSIZE + 3 * LINESPACING));
  for (auto it = _history.content.cbegin() + _history.pos;
       it != _history.content.cend() && wSize < cSize; ++it)
    {
      _text.setString(*it);
      window.draw(_text);
      _text.move(0, -(FONTSIZE + LINESPACING));
      wSize += (FONTSIZE + LINESPACING);
    }
  _text.setPosition(0, cSize);
  _text.setString("]" + _input.getString());
  window.draw(_text);
}
