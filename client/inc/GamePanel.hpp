#ifndef _GAMEPANEL_H_
#define _GAMEPANEL_H_

#include "Network.hpp"
#include "NetworkException.hpp"
#include "Panel.hpp"
#include "Widget.hpp"
#include "World.hpp"
#include "ClientProtocol.hpp"
#include "ActionAnalyzer.hpp"

#define ITERATIONS 5
#define MHEIGHT 300
#define SHEIGHT 500.f

class GamePanel : public APanelScreen
{
public:
  GamePanel(const sf::FloatRect &zone);
  virtual ~GamePanel();

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);

  void		draw(sf::RenderWindow &window, bool toWin);
  int		update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
  void		trigger(const t_event &event);

private:
  void		createButton(const sf::Texture &texture, Widget *w);
  void		createVoiceButton(const sf::Texture &texture, Widget *w,
				  Controls &controls, int idx);
  void		constructLine();
  void		completeField(sf::RenderWindow &window);
  void		fillVertex(sf::Vector2f &prev, sf::Vector2f &next, int x);
  void		drawHud(sf::RenderWindow &window, bool toWin);
  int		updateHud(const sf::Event &event, sf::RenderWindow &ref, Settings &set);

  int		updateNetwork();
  void		handlePackets(ENetEvent &event);
  void		connectClient(ENetPeer * const peer);
  void		disconnectClient(ENetPeer * const peer);

  int		_pad;
  int		_padup;
  int		_oldY;
  bool		_dir;
  std::shared_ptr<World>	_world;
  Network		_socket;
  ClientProtocol	_proto;
  ActionAnalyzer	_actAnalyzer;
};

#endif /* _GAMEPANEL_H_ */
