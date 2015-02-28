#include <fstream>
#include <iostream>
#include <SFML/Audio.hpp>
#include "GamePanel.hpp"
#include "SimplexNoise.h"
#include "FontManager.hpp"

GamePanel::GamePanel(const sf::FloatRect &zone) :
  APanelScreen(zone), _pad(0), _padup(0),
  _oldY(SHEIGHT),
  _dir(true),
  _threadPool(4),
  _chat(),
  _world(nullptr), _socket(), _proto(_socket, _threadPool, _chat),
  _actAnalyzer(), _adjustedNet(false)
{
  addFont("default", "../client/assets/default.TTF");
  setHide(true);
}

GamePanel::~GamePanel()
{
  _socket.disconnectPeer(Moment::Now, 0);
  _socket.disconnect();
}

void	GamePanel::construct(const sf::Texture &texture UNUSED, Settings &set UNUSED,
			     const std::vector<APanelScreen *> &panels)
{
  Controls	&controls = set.getControls();
  auto		&fm = FontManager<>::instance();
  float	gWidth = std::stof(set.getCvarList().getCvar("r_width"));
  float gHeight = std::stof(set.getCvarList().getCvar("r_height"));
  sf::FloatRect panZone = {gWidth / 2 - 150, gHeight / 2 - 250, 300, 500};

  Panel		*pSound = new Panel(panZone);
  Widget	*wHeader = new Widget("Header", {panZone.left, panZone.top, panZone.width, 60},
				      sf::Text("Header", _font["default"], 30));
  Widget	*wFirst = new Widget("sound1", {panZone.left, panZone.top + 57, panZone.width, 60},
				     sf::Text("1.", _font["default"], 22));
  Widget	*wSecond = new Widget("sound2", {panZone.left, panZone.top + 114, panZone.width, 60},
				      sf::Text("2.", _font["default"], 22));
  Widget	*wThird = new Widget("sound3", {panZone.left, panZone.top + 171, panZone.width, 60},
				     sf::Text("3.", _font["default"], 22));
  Widget	*wFourth = new Widget("sound4", {panZone.left, panZone.top + 228, panZone.width, 60},
				      sf::Text("4.", _font["default"], 22));
  TextWidget	*input = new TextWidget("", sf::FloatRect(_zone.left + _zone.width / 2 - 300
							  , _zone.top + _zone.height / 2 - 25,
							  600, 50),
					sf::Text("", *fm.get("Title-font.ttf"), 20),
					sf::Text("", *fm.get("Title-font.ttf"), 20), 60);

  addObserver(panels[0]);
  createMessageEntry(texture, set, input);
  createButton(texture, wHeader);
  createVoiceButton(texture, wFirst, controls, sf::Keyboard::Num1);
  createVoiceButton(texture, wSecond, controls, sf::Keyboard::Num2);
  createVoiceButton(texture, wThird, controls, sf::Keyboard::Num3);
  createVoiceButton(texture, wFourth, controls, sf::Keyboard::Num4);
  wFirst->addObserver(pSound);
  wSecond->addObserver(pSound);
  wThird->addObserver(pSound);
  wFourth->addObserver(pSound);

  pSound->addWidget({wHeader, wFirst, wSecond, wThird, wFourth});
  pSound->construct(texture, set, panels);
  pSound->setHide(true);
  addPanel(pSound);
  addWidget(input);
  _world.reset(new World{set});
  resizeWidgets({gWidth, gHeight});
  _proto.setSetting(&set);
  _proto.setWorld(_world);
}

void	GamePanel::createButton(const sf::Texture &texture, Widget *w)
{
  sf::FloatRect	zone = w->getZone();

  w->alignTextLeft({zone.left,zone.top}, {zone.width, zone.height}, 8, 50);
  w->addSprite(texture, sf::IntRect(0, 1080, 260, 60));
}

void	GamePanel::createMessageEntry(const sf::Texture &texture UNUSED,
				      Settings &set,
				      TextWidget *widget)
{
  sf::FloatRect zone = widget->getZone();
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [this, &set](AWidget &widget, const sf::Event &ev UNUSED,
				 sf::RenderWindow &ref UNUSED)
    -> int
    {
      TextWidget	*twidget = dynamic_cast<TextWidget *>(&widget);
      sf::FloatRect	wZone = widget.getZone();
      std::string	content = twidget->getContent();
      bool		hideState = widget.isHidden();
      Controls		&controls = set.getControls();

      widget.alignTextLeft({wZone.left,wZone.top}, {wZone.width, wZone.height}, 1, 50);
      if (twidget->getState() == false && !content.empty())
	{
	  t_entry		entry = controls.getKeyFromAction(Action::Chat);
	  ClientMessage		msg;
	  std::string		serialized;

	  msg.set_content(ClientMessage::CHAT);
	  msg.set_chat(set.getCvarList().getCvar("cl_name") + ": " + content);
	  msg.SerializeToString(&serialized);

	  _socket.sendPacket(1, serialized);
	  twidget->clearWidget();
	  controls.pressKey(entry);
	}
      widget.setHidden(!controls.getActionState(Action::Chat));
      if (hideState == true && widget.isHidden() == false) // will appear next frame
	twidget->setState(true);
      return 0;
    };
  addSpriteForWidget(widget, sf::Color(255, 255, 255, 200), {zone.width, zone.height});
  widget->setUpdate(updateFunc);
  widget->setColor(sf::Color(0,0,0));
  widget->getCursor().setColor(sf::Color(0,0,0));
  widget->setEdge(sf::Vector2f(zone.width, zone.height),
		  2.f);
  widget->setHidden(true);
}

void	GamePanel::createVoiceButton(const sf::Texture &texture, Widget *w,
				     Controls &controls, int idx)
{
  sf::FloatRect	zone = w->getZone();
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [&controls, idx](AWidget &widget UNUSED, const sf::Event &ev,
				sf::RenderWindow &ref UNUSED)
    -> int
    {
      if (ev.type == sf::Event::KeyPressed && ev.key.code == idx)
	{
	  t_entry		entry = controls.getKeyFromAction(Action::ToggleQuickMenu);
	  sf::SoundBuffer	buffer;
	  sf::Sound		sound;

	  controls.pressKey(entry);
	  controls.releaseKey(entry);
	  if (!buffer.loadFromFile("../client/assets/sound1.wav"))
	    return 1;
	  sound.setBuffer(buffer);
	  sound.play();
	  sf::sleep(sf::milliseconds(1000));
	  return 1;
	}
      return 0;
    };
  w->setUpdate(updateFunc);
  w->alignTextLeft({zone.left,zone.top}, {zone.width, zone.height}, 8, 50);
  w->addSprite(texture, sf::IntRect(0, 1080, 260, 60));
}

void	GamePanel::drawHud(sf::RenderTarget &window, bool toWin)
{
  APanelScreen::draw(window, toWin);
  _chat.draw(window);
}

void	GamePanel::draw(sf::RenderTarget &window, bool toWin)
{
  _world->draw(window);
  drawHud(window, toWin);
}

void	GamePanel::trigger(const t_event &ev)
{
  if (ev.e & ~wEvent::Update) // important to switch the hide state before connecting
    APanelScreen::trigger(ev);
  if (ev.e & wEvent::Update)
    {
      _hide = false;
      try
	{
	  const std::string defaultPort("6060");
	  std::string	ip(ev.str);
	  size_t nbColon = std::count(ip.begin(), ip.end(), ':');
      std::string realIp = (nbColon % 2 == 0 ? ip : ip.substr(0, ip.find_last_of(':')));
      std::string realPort = (nbColon % 2 == 1 ? ip.substr(ip.find_last_of(':') + 1) : defaultPort);

	  std::cout << "GamePanel trigger " << realIp << ":" << realPort << std::endl;
	  _adjustedNet = false;
	  _socket.connect(realIp, realPort, 3);
	}
      catch (NetworkException &e)
	{
	  std::cerr << "Cannot connect to ip : " << e.what() << std::endl;
	  setHide(true);
	  notify(t_event(wEvent::Hide | wEvent::Toggle));
	}
    }
  else
    APanelScreen::trigger(ev);
}

int	GamePanel::updateHud(const sf::Event &ev, sf::RenderWindow &ref, Settings &set)
{
  int	retVal = 0;

  //  (getSubPanels()[0])->setHide(!set.getControls().getActionState(Action::ToggleQuickMenu));
  for (auto rit = _panels.rbegin(); rit != _panels.rend(); ++rit)
    {
      if ((*rit)->isHidden() == false)
	if ((retVal = (*rit)->event(ev, ref, set)) != 0)
	  return retVal;
    }
  for (auto rit = _widgets.rbegin(); rit != _widgets.rend(); ++rit)
    {
      if ((retVal = (*rit)->update(ev, ref, set)) != 0)
	return retVal;
    }
  return 0;
}

void		GamePanel::adjustNetworkSettings(Settings &set)
{
  const CvarList	&cvars = set.getCvarList();
  unsigned int		upBandwidth = std::stoul(cvars.getCvar("net_upBandwidth"));
  unsigned int		downBandwidth = std::stoul(cvars.getCvar("net_downBandwidth"));

  _socket.adjustNetworkSettings(downBandwidth, upBandwidth);
  _adjustedNet = true;
}

int		GamePanel::updateNetwork(Settings &set)
{
  ENetEvent	ev;
  bool		pull = true;
  int		haveEvent;

  try
    {
      while ((haveEvent = _socket.pullEvent(ev, 0, pull)) > 0)
	{
	  switch (ev.type)
	    {
	    case ENET_EVENT_TYPE_CONNECT:
	      connectClient(ev.peer, set);
	      break;
	    case ENET_EVENT_TYPE_RECEIVE:
	      this->handlePackets(ev);
	      break;
	    case ENET_EVENT_TYPE_DISCONNECT:
	      disconnectClient(ev.peer);
	      break;
	    default:
	      break;
	    }
	}
      if (haveEvent < 0)
	throw NetworkException("Connection problem");
    }
  catch (NetworkException &e)
    {
      std::cerr << e.what() << std::endl;
    }
  return 0;
}

void	GamePanel::handlePackets(ENetEvent &ev)
{
  _proto.parseCmd(ev.packet->data, ev.packet->dataLength);
  enet_packet_destroy(ev.packet);
}

void	GamePanel::connectClient(ENetPeer * const peer, Settings &set)
{
  peer->data = (char *)("Server");
  sendConnectionInfo(set);
}

void	GamePanel::disconnectClient(UNUSED ENetPeer * const peer)
{
  std::cout << "Disconnect Peer" << std::endl;
  setHide(true);
  notify(t_event(wEvent::Hide | wEvent::Toggle));
}

void			GamePanel::getGuidFromFile(std::string *guid) const
{
  std::ifstream		file;

  file.open(GUIDFILE, std::ofstream::binary | std::ofstream::in);
  if (file)
    {
      file >> (*guid);
      file.close();
    }
}

void			GamePanel::sendConnectionInfo(Settings &set)
{
  ClientMessage		msg;
  ConnectionMessage	*co = new ConnectionMessage;
  std::string		*userId = new std::string;
  std::string		*name = new std::string(set.getCvarList().getCvar("cl_name"));
  std::string		serialized;

  getGuidFromFile(userId);
  co->set_allocated_userid(userId);
  co->set_allocated_name(name);
  msg.set_content(ClientMessage::CONNECTION);
  msg.set_allocated_co(co);
  msg.SerializeToString(&serialized);

  _socket.sendPacket(1, serialized);
}

void	GamePanel::update(std::chrono::milliseconds timeStep, Settings &set)
{
  if (_hide)
    return ;
  if (_actAnalyzer.getInputChanges(set))
    _socket.sendPacket(1, _actAnalyzer.serialize());
  updateNetwork(set);
  _chat.update();
  _world->update(timeStep);
}

int		GamePanel::event(const sf::Event &ev,
				  sf::RenderWindow &ref,
				  Settings &set)
{
  int		retVal;

  retVal = updateHud(ev, ref, set);
  return retVal;
}
