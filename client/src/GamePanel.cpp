#include <fstream>
#include <iostream>
#include <SFML/Audio.hpp>
#include "GamePanel.hpp"
#include "SimplexNoise.h"

GamePanel::GamePanel(const sf::FloatRect &zone) :
  APanelScreen(zone), _pad(0), _padup(0),
  _oldY(SHEIGHT), _dir(true),  _threadPool(15),
  _world(nullptr), _socket(), _proto(_socket, _threadPool),
  _actAnalyzer()
{
  addFont("default", "../client/assets/default.TTF");
  setHide(true);
}

GamePanel::~GamePanel()
{
  _socket.disconnect();
}

void	GamePanel::construct(const sf::Texture &texture UNUSED, Settings &set UNUSED,
			     const std::vector<APanelScreen *> &panels)
{
  Controls	&controls = set.getControls();
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

  addObserver(panels[0]);
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
  addPanels({pSound});
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

void	GamePanel::drawHud(sf::RenderWindow &window, bool toWin)
{
  APanelScreen::draw(window, toWin);
}

void	GamePanel::draw(sf::RenderWindow &window, bool toWin)
{
  _world->draw(window);
  drawHud(window, toWin);
}

void	GamePanel::trigger(const t_event &ev)
{
  if (ev.e & wEvent::Hide)
    {
      if (ev.e & wEvent::Toggle)
	{
	  _hide = !_hide;
	  if (_hide == false)
	    {
	      try
		{
		  _socket.connect("127.0.0.1", "6060", 2);
		}
	      catch (NetworkException &e)
		{
		  std::cerr << e.what() << std::endl;
		}
	    }
	}
      else
	_hide = true;
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

int		GamePanel::updateNetwork(Settings &set)
{
  ENetEvent	ev;

  try
    {
      if (_socket.pollEvent(&ev, 1) < 0)
	throw NetworkException("Connection problem");
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

void	GamePanel::connectClient(ENetPeer * const peer, Settings &set UNUSED)
{
  peer->data = (char *)("Server");
  sendConnectionInfo();
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

void			GamePanel::sendConnectionInfo() const
{
  ClientMessage		msg;
  ConnectionMessage	*co = new ConnectionMessage;
  std::string		*userId = new std::string;
  std::string		serialized;

  getGuidFromFile(userId);
  co->set_allocated_userid(userId);
  msg.set_content(ClientMessage::CONNECTION);
  msg.set_allocated_co(co);
  msg.SerializeToString(&serialized);

  _socket.sendPacket(1, serialized);
}

int		GamePanel::event(const sf::Event &ev,
				  sf::RenderWindow &ref,
				  Settings &set)
{
  int		retVal;

  updateNetwork(set);
  if (_actAnalyzer.getInputChanges(set))
    {
      _socket.sendPacket(1, _actAnalyzer.serialize());
    }
  retVal = updateHud(ev, ref, set);
  _world->update();
  return retVal;
}
