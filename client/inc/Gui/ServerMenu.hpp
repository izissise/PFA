#ifndef _SERVERMENU_H_
#define _SERVERMENU_H_

#include <mutex>
#include <queue>

#include "Panel.hpp"
#include "Widget.hpp"
#include "TextWidget.hpp"
#include "TabWidget.hpp"
#include "ScrollWidget.hpp"
#include "ServerItem.hpp"
#include "Network.hpp"
#include "MasterServerRequest.pb.h"
#include "MasterServerResponse.pb.h"

#define FavFile "../favlist.txt"

typedef struct	s_server
{
  APanelScreen	*container;
  APanelScreen	*list;
  ServerInfo	packet;
  s_server(APanelScreen *cont, APanelScreen *l,
	   const ServerInfo &p) :
    container(cont), list(l), packet(p)
  {
  }
}		t_server;

class ServerMenu : public APanelScreen
{
public:
  ServerMenu(const sf::FloatRect &zone);
  virtual ~ServerMenu();

  void		update(std::chrono::milliseconds timeStep, Settings &set);
  int		event(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);
  void		trigger(const t_event &event);

private:
  int		updateHud(const sf::Event &ev, sf::RenderWindow &ref, Settings &set);
  void		parseServerPacket(Settings &set, const void *data, int size);
  void		updateContent(Settings &set);
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
  Panel		*createServerPopup(Settings &set, const sf::Texture &texture,
				   const std::vector<APanelScreen *> &panels);
  void		createPopupText(Widget *widget, bool left);
  void		createPopupJoin(Widget *widget, Widget *ip);
  void		setFavTrigger(Settings &set, const sf::Texture &texture,
			      Panel *panel);
  void		loadFavServers(Settings &set, const sf::Texture &texture,
			       Panel *panel);
  bool		addServerToFav(const std::string &ip) const;
  bool		removeServerFromFav(const std::string &ip) const;
  ServerItem	*addServerToList(Settings &set,
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
  bool		_update;
  Network	_masterSocket;
  Panel		*_panelCo;
  Panel		*_cont;
  std::mutex	_mutex;
  const sf::Texture		*_texture;
  std::queue<t_server>		_servers;
  std::queue<std::string>	_messages;
};

#endif /* _SERVERMENU_H_ */
