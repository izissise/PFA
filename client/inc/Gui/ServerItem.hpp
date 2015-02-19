#ifndef _SERVERITEM_H_
#define _SERVERITEM_H_

#include <SFML/System/Clock.hpp>
#include <queue>
#include <mutex>

#include "Panel.hpp"
#include "Widget.hpp"
#include "Network.hpp"
#include "ClientMessage.pb.h"

class ServerItem : public APanelScreen
{
public:
  ServerItem(const sf::FloatRect &zone, const std::string &ip);
  virtual ~ServerItem() = default;

  void	construct(const sf::Texture &texture, Settings &set,
		  const std::vector<APanelScreen *> &panels);
  int	event(const sf::Event &ev, sf::RenderWindow &ref, Settings &set);
  void	update(UNUSED std::chrono::milliseconds timeStep, UNUSED Settings &set);
  void	updateItem(const std::string &info,
		   const std::string &value);

  const std::string	&getIp() const;
private:
  void	createBackgroundController(Widget *widget);

private:
  std::string	_ip;
  Network	_socket;
  sf::Clock	_time;
  std::queue<uint64_t>	_pingTime;
  std::mutex	_mutex;
};

#endif /* _SERVERITEM_H_ */
