#ifndef _SERVERINFOPANEL_H_
#define _SERVERINFOPANEL_H_

#include "APanelScreen.hpp"
#include "Widget.hpp"

class ServerInfoPanel : public APanelScreen
{
public:
  ServerInfoPanel(const sf::FloatRect &zone,
	      const std::string &ip);

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);

private:
  void		createNameWidget(Widget *widget);
  void		createCountryWidget(Widget *widget);
  void		createPlayerWidget(Widget *widget);
  void		createPingWidget(Widget *widget);
  void		applyButtonStyle(Widget *widget);

private:
  std::string		_ip;
};

#endif /* _SERVERINFOPANEL_H_ */
