#include <functional>
#include "Settings.hpp"
#include "Console.hpp"
#include "Exception.hpp"

Console::Console(Settings &set, Parser *parser) :
  _cursor(), _historical(), _parser(parser)
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

void	Console::getInput()
{
  const std::vector<std::string> &output = _parser->parseLine(_input.getString().toAnsiString());
  _historical.addString(_input.getString());
  _input.clear();
  for (auto &it : output)
    _historical.addString(it);
}

void		Console::run(const sf::Event& event, Controls &ctrl)
{
  t_entry	entry;
  std::function<bool (Action act,
		      const std::initializer_list<Action> &actions)> isConsoleKey =
    [](Action act, const std::initializer_list<Action> &actions)
    {
      return std::find(actions.begin(), actions.end(), act) != actions.end();
    };

  if (_input.getInput(event)) // means enter got pressed
    getInput();
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
	   && sf::Keyboard::isKeyPressed(sf::Keyboard::L))
    _historical.clear();
  _text.setString("]" + _input.getString());
  _cursor.update();
  _cursor.setCursorPos(_text);
  entry.fill(event);
  switch (event.type)
    {
    case sf::Event::KeyPressed:
      if (isConsoleKey(ctrl.getActionFromKey(entry), {Action::ToggleConsole}))
	ctrl.pressKey(entry);
      break;
    case sf::Event::KeyReleased:
      if (isConsoleKey(ctrl.getActionFromKey(entry), {Action::ToggleConsole}))
	ctrl.releaseKey(entry);
      break;
    case sf::Event::MouseWheelMoved:
      _historical.setPos((static_cast<int>(_historical.getPos()) + event.mouseWheel.delta < 0) ? 0 :
			 (_historical.getPos() + event.mouseWheel.delta > _historical.getSize()) ?
			 _historical.getSize() :
			 _historical.getPos() + event.mouseWheel.delta);
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
  for (auto it = _historical.getContent().cbegin() + _historical.getPos();
       it != _historical.getContent().cend() && wSize < cSize; ++it)
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
