#include "Settings.hpp"
#include "Console.hpp"
#include "Exception.hpp"

Console::Console(Settings &set, Parser *parser) :
  _cursor(), _history(), _parser(parser)
{
  sf::Color	color(20, 20, 20);
  int		width = std::stoi(set.getCvarList().getCvar("r_width"));
  int		height = std::stoi(set.getCvarList().getCvar("r_height"));

  _rectangle.setSize(sf::Vector2f(width, height / 2));
  _rectangle.setPosition(0, 0);
  _rectangle.setOutlineColor(sf::Color::White);
  _rectangle.setOutlineThickness(2);
  _rectangle.setFillColor(color);

  if (!_font.loadFromFile("../client/assets/default.TTF"))
    std::cerr << "Can't load font" << std::endl; // replace this by a throw about ressources
  _text.setFont(_font);
  _text.setStyle(sf::Text::Regular);
  _text.setColor(sf::Color::White);
  _text.setCharacterSize(FONTSIZE);
  _cursor.setText(sf::Text("|", _font, FONTSIZE));
}

Console::~Console()
{
}


void		Console::run(const sf::Event& event, Controls &ctrl)
{
  t_entry	entry;

  if (_input.getInput(event))
    {
      try
	{
	  _parser->parseLine(_input.getString().toAnsiString());
        }
      catch (const Exception &e)
	{
          std::cerr << e.what() << std::endl;
        }
      _history.content.push_front(_input.getString());
      if (_history.content.size() > _history.maxSize)
        _history.content.pop_back();
      _input.clear();
    }
  _text.setString("]" + _input.getString());
  _cursor.update();
  _cursor.setCursorPos(_text);
  entry.fill(event);
  switch (event.type)
    {
    case sf::Event::KeyPressed:
      ctrl.pressKey(entry);
      break;
    case sf::Event::KeyReleased:
      ctrl.releaseKey(entry);
      break;
    case sf::Event::MouseWheelMoved:
      _history.pos =
        ((static_cast<int>(_history.pos) + event.mouseWheel.delta < 0) ? 0 :
         (_history.pos + event.mouseWheel.delta > _history.content.size()) ?
         _history.content.size() :
         _history.pos + event.mouseWheel.delta);
      break;
    default:
      break;
    }
}

void		Console::draw(sf::RenderWindow &window)
{
  unsigned int	wSize = (FONTSIZE + 2 * LINESPACING);
  unsigned int	cSize;
  sf::FloatRect	rect;

  rect = _rectangle.getGlobalBounds();
  cSize = rect.height - wSize;
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
  _cursor.draw(window);
}
