#ifndef _SERVERPANEL_H_
#define _SERVERPANEL_H_

#include "APanelScreen.hpp"

class ServerPanel : public APanelScreen
{
public:
  ServerPanel(const sf::FloatRect &zone,
	      const std::string &ip);

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);

private:
  void		createNameWidget(Widget *widget);
  void		createCountryWidget(Widget *widget);
  void		createPlayerWidget(Widget *widget);
  void		createPingWidget(Widget *widget);
  void		applyButtonStyle(Widget *widget, unsigned int id);

private:
  std::string		_ip;
};

#endif /* _SERVERPANEL_H_ */
