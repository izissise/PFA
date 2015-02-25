#ifndef _GAMEPANEL_H_
#define _GAMEPANEL_H_

#include "Panel.hpp"
#include "Widget.hpp"
#include "World.hpp"
#include "ClientProtocol.hpp"
#include "ActionAnalyzer.hpp"
#include "GamePanel.hpp"
#include "TextWidget.hpp"

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

  void		update(std::chrono::milliseconds timeStep, Settings &set) override;
  void		draw(sf::RenderTarget &window, bool toWin) override;
  int		event(const sf::Event &ev, sf::RenderWindow &ref, Settings &set) override;

  void		trigger(const t_event &event);

private:
  void		createMessageEntry(const sf::Texture &texture, Controls &controls, TextWidget *widget);
  void		createButton(const sf::Texture &texture, Widget *w);
  void		createVoiceButton(const sf::Texture &texture, Widget *w,
				  Controls &controls, int idx);
  void		drawHud(sf::RenderTarget &window, bool toWin);
  int		updateHud(const sf::Event &event, sf::RenderWindow &ref, Settings &set);

  void		adjustNetworkSettings(Settings &set);
  int		updateNetwork(Settings &set);
  void		handlePackets(ENetEvent &event);
  void		connectClient(ENetPeer * const peer, Settings &set);
  void		disconnectClient(ENetPeer * const peer);

  void		sendConnectionInfo() const;
  void		getGuidFromFile(std::string *guid) const;

  int		_pad;
  int		_padup;
  int		_oldY;
  bool		_dir;
  ThreadPool		_threadPool;
  std::shared_ptr<World>	_world;
  Network		_socket;
  ClientProtocol	_proto;
  ActionAnalyzer	_actAnalyzer;
  bool			_adjustedNet;
};

#endif /* _GAMEPANEL_H_ */
