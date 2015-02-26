#include "Chat.hpp"
#include "FontManager.hpp"
#include "Widget.hpp"

void	Chat::update()
{
  std::lock_guard<std::mutex> lock(_mutex);
  bool	erased = true;

  while (erased && !_messages.empty())
    {
      t_chatElem	&elem = _messages.front();

      if (elem.time.getElapsedTime().asMilliseconds() > Chat::timeDelay)
	{
	  _messages.pop_front();
	  erased = true;
	}
      else
	erased = false;
    }
}

void	Chat::draw(sf::RenderTarget &window)
{
  std::lock_guard<std::mutex>	lock(_mutex);
  const sf::Vector2u	&size = window.getSize();
  unsigned int		count = 0;
  auto			&fm = FontManager<>::instance();
  Widget		widget("", sf::FloatRect(0,size.y,size.x, 40),
			       sf::Text("", *fm.get("default.TTF")));

  for (auto &elem : _messages)
    {
      widget.move(0, -40);
      const sf::FloatRect &zone = widget.getZone();

      widget.alignText(sf::Vector2f(zone.left, zone.top),
		       sf::Vector2f(zone.width, zone.height),
		       50, 50);
      widget.draw(window);
      ++count;
    }
}

void	Chat::addMessages(const std::string &str)
{
  std::lock_guard<std::mutex>	lock(_mutex);
  _messages.push_back(t_chatElem(str));
}
