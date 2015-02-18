#ifndef _SERVERITEM_H_
#define _SERVERITEM_H_

#include "Panel.hpp"
#include "Widget.hpp"

class ServerItem : public APanelScreen
{
public:
  ServerItem(const sf::FloatRect &zone, const std::string &ip);
  virtual ~ServerItem() = default;

  void	construct(const sf::Texture &texture, Settings &set,
		  const std::vector<APanelScreen *> &panels);
  void	updateItem(const std::string &info,
		   const std::string &value);

  const std::string	&getIp() const;
private:
  void	createBackgroundController(Widget *widget);

private:
  std::string	_ip;
};

#endif /* _SERVERITEM_H_ */
