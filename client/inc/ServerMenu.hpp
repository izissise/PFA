#ifndef _SERVERMENU_H_
#define _SERVERMENU_H_

#include "Panel.hpp"
#include "Widget.hpp"
#include "TextWidget.hpp"

class ServerMenu : public APanelScreen
{
public:
  ServerMenu(const sf::FloatRect &zone);
  virtual ~ServerMenu();

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);

private:
  void		createTitle(Widget *title);
  void		createFooter(Widget *footer);
  void		createContPanel(Settings &set, const sf::Texture &texture,
				const std::vector<APanelScreen *> &panels);
  void		createCoPopup(Settings &set, const sf::Texture &texture,
			      const std::vector<APanelScreen *> &panels,
			      Widget *linkWidget);
  void		createButtonStyle(Widget *widget, const sf::Texture &texture);
  void		createButtonBack(Widget *widget, const sf::Texture &texture);
  void		createButtonJoin(Widget *widget, const sf::Texture &texture);
  void		createButtonCip(Widget *widget, const sf::Texture &texture);
  void		createTextWidget(TextWidget *widget, const sf::Texture &texture);
  void		createCancelButton(Widget *widget, const sf::Texture &texture);
  void		createConnectButton(Widget *widget, const sf::Texture &texture);
};

#endif /* _SERVERMENU_H_ */
