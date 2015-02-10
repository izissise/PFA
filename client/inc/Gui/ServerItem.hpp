#ifndef _SERVERITEM_H_
#define _SERVERITEM_H_

#include "Panel.hpp"
#include "Widget.hpp"

class ServerItem : public APanelScreen
{
public:
  ServerItem(const sf::FloatRect &zone);
  virtual ~ServerItem() = default;

  void	construct(const sf::Texture &texture, Settings &set,
		  const std::vector<APanelScreen *> &panels);
  void	updateItem(const std::vector<std::string> &info);

private:
  void	createBackgroundController(Widget *widget);
};

#endif /* _SERVERITEM_H_ */
