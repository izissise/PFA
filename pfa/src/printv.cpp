#include <SFML/Graphics.hpp>
#include "printv.hpp"

std::ostream& printv(std::ostream& f, const std::string& s)
{
  size_t prevPos;;
  size_t pos;

  pos = 0;
  prevPos = 0;
  while ((pos = s.find('%', prevPos)) != std::string::npos)
    {
      f << s.substr(prevPos, pos - prevPos);
      if (s[pos + 1] != '%')
        throw std::runtime_error("invalid format string: missing arguments");
      else
        {
          ++pos;
          f << '%';
        }
      prevPos = pos + 1;
    }
  f << s.substr(prevPos);
  return f;
}

void testSfml()
{
  sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

  while (window.isOpen())
    {
      sf::Event event;
      while (window.pollEvent(event))
        {
          if (event.type == sf::Event::Closed)
            window.close();
        }

      window.clear();
      window.draw(shape);
      window.display();
    }
}
