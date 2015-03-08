#ifndef _CHAT_H_
#define _CHAT_H_

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <mutex>
#include <string>
#include <deque>

typedef struct	s_chatElem
{
  std::string	msg;
  sf::Clock	time;

  s_chatElem(const std::string &m) :
    msg(m), time()
  {
  }
}		t_chatElem;

class Chat
{
private:
  static const int	timeDelay = 8000;
public:
  Chat() = default;
  virtual ~Chat() = default;

  void	update();
  void	draw(sf::RenderTarget &window);
  void	addMessages(const std::string &str);

private:
  std::deque<t_chatElem>	_messages;
  std::mutex			_mutex;
};

#endif /* _CHAT_H_ */
