#include "Console.hpp"
#include "Exception.hpp"

Console::Console(Settings * const set) : _set(set)
{
  sf::Color	color(20, 20, 20);

  _rectangle.setSize(sf::Vector2f(std::stoi(set->getCvarList().getCvar("r_width")),
				  std::stoi(set->getCvarList().getCvar("r_height")) / 2));
  _rectangle.setPosition(0, 0);
  _rectangle.setOutlineColor(sf::Color::White);
  _rectangle.setOutlineThickness(2);
  _rectangle.setFillColor(color);

  if (!_font.loadFromFile("../client/assets/font.otf"))
    std::cerr << "Can't load font" << std::endl; // replace this by a throw about ressources
  _text.setFont(_font);
  _text.setStyle(sf::Text::Regular);
  _text.setColor(sf::Color::White);
  _text.setCharacterSize(20);
}

Console::~Console()
{
}


void		Console::run(sf::RenderWindow &window, sf::Event event)
{
  Controls	&ctrl = _set->getControls();

  _text.setString(_input.getString());
  if (_input.getInput(event))
    {
      try {
	_set->parseCommandLine(_input.getString().toAnsiString());
      }
      catch (const Exception &e) {
	std::cerr << e.what() << std::endl;
      }
      _input.clear();
    }
  if (event.type == sf::Event::KeyPressed)
    ctrl.pressKey(event.key.code);
  else if (event.type == sf::Event::KeyReleased)
    ctrl.releaseKey(event.key.code);
}

void		Console::draw(sf::RenderWindow &window) const
{
  window.draw(_rectangle);
  window.draw(_text);
}
