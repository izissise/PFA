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
			       sf::Text("", *fm.get("default.TTF"), 25));
  const sf::FloatRect	&zone = widget.getZone();

  for (auto it = _messages.rbegin(); it != _messages.rend(); ++it)
    {
      t_chatElem	&elem = *it;
      std::size_t	elapsed = elem.time.getElapsedTime().asMilliseconds();

      if (elapsed > Chat::timeDelay)
	elapsed = Chat::timeDelay;
      if (elapsed > Chat::timeDelay / 2)
	widget.setColor(sf::Color(0,0,0, 255 - ((elapsed - Chat::timeDelay / 2) * 255) /
				  (Chat::timeDelay / 2)));
      else
	widget.setColor(sf::Color(0,0,0,255));
      widget.setTextContent(elem.msg);
      widget.move(0, -40);
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
