#ifndef _SERVERMENU_H_
#define _SERVERMENU_H_

#include "Panel.hpp"
#include "Widget.hpp"
#include "TextWidget.hpp"
#include "TabWidget.hpp"
#include "ScrollWidget.hpp"

#define FavFile "../favlist.txt"

class ServerMenu : public APanelScreen
{
public:
  ServerMenu(const sf::FloatRect &zone);
  virtual ~ServerMenu();

  int		update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);
  void		trigger(const t_event &event);

private:
  int		updateView(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
  void		updateContent();
  void		createHeader(Settings &set, const sf::Texture &texture,
			     const std::vector<APanelScreen *> &panels);
  void		createTitle(Widget *title, const sf::FloatRect &zone);
  void		createHome(Widget *home, const sf::FloatRect &zone);
  void		createIp(Widget *widget, const sf::FloatRect &zone);
  void		createListHeader(Settings &set, const sf::Texture &texture,
				 const std::vector<APanelScreen *> &panels);
  void		createPopupHeader(Widget *widget);
  Panel		*createContPanel(Settings &set, const sf::Texture &texture,
				 const std::vector<APanelScreen *> &panels);
  Panel		*createServListPanel(Settings &set, const sf::Texture &texture,
				     const std::vector<APanelScreen *> &panels);
  Panel		*createFavPanel(Settings &set, const sf::Texture &texture,
			       const std::vector<APanelScreen *> &panels);
  void		createTabBar(Settings &set, const sf::Texture &texture,
			     const std::vector<APanelScreen *> &panels);
  Panel		*createCoPopup(Settings &set, const sf::Texture &texture,
			      const std::vector<APanelScreen *> &panels);
  Panel		*createServerPanel(Settings &set, const sf::Texture &texture,
				   const std::vector<APanelScreen *> &panels,
				   const sf::FloatRect &zone,
				   const std::string &ip);
  Panel		*createServerPopup(Settings &set, const sf::Texture &texture,
				   const std::vector<APanelScreen *> &panels);
  void		createServerPopupText(Widget *widget);
  void		createPopupControler(Widget *widget, const std::vector<APanelScreen *> &panels);
  void		setControlerTrigger(Panel *controler);
  void		setFavTrigger(Settings &set, const sf::Texture &texture,
			      Panel *panel, APanelScreen *container);
  void		loadFavServers(Settings &set, const sf::Texture &texture,
			       Panel *panel, APanelScreen *container);
  bool		addServerToFav(const std::string &ip) const;
  bool		removeServerFromFav(const std::string &ip) const;
  void		addServerToList(Settings &set,
				const sf::Texture &texture,
				const std::string &ip,
				const std::vector<APanelScreen *> &panels);
  void		createTabButton(TabWidget *widget);
  void		createButtonStyle(Widget *widget, const sf::Texture &texture);
  void		createTextWidget(TextWidget *widget, const sf::Texture &texture);
  void		createCancelButton(Widget *widget, const sf::Texture &texture);
  void		createConnectButton(Widget *widget, const sf::Texture &texture);
  void		createAddFavButton(const sf::String &ip, Widget *widget,
				   const sf::Texture &texture);
  void		createRemFavButton(const sf::String &ip, Widget *widget,
				   const sf::Texture &texture);
  void		createScrollBar(ScrollWidget *widget, const sf::Texture &texture);
  void		setPopupTrigger(Panel *panel);
  void		setServerPopupTrigger(Panel *panel);

private:
  unsigned int	_frameCount;
};

#endif /* _SERVERMENU_H_ */
