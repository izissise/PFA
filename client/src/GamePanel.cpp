#include <iostream>
#include <SFML/Audio.hpp>
#include "GamePanel.hpp"
#include "SimplexNoise.h"

GamePanel::GamePanel(const sf::FloatRect &zone) :
  APanelScreen(zone), _pad(0), _padup(0),
  _oldY(SHEIGHT), _dir(true), _world(nullptr),
  _socket(), _proto(), _actAnalyzer()
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
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [&controls, idx](AWidget &widget UNUSED, const sf::Event &event,
				sf::RenderWindow &ref UNUSED)
    -> int
    {
      if (event.type == sf::Event::KeyPressed && event.key.code == idx)
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

void	GamePanel::trigger(const t_event &event)
{
  if (event.e & wEvent::Hide)
    {
      if (event.e & wEvent::Toggle)
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
    APanelScreen::trigger(event);
}

int	GamePanel::updateHud(const sf::Event &event, sf::RenderWindow &ref, Settings &set)
{
  int	retVal = 0;

  //  (getSubPanels()[0])->setHide(!set.getControls().getActionState(Action::ToggleQuickMenu));
  for (auto rit = _panels.rbegin(); rit != _panels.rend(); ++rit)
    {
      if ((*rit)->isHidden() == false)
	if ((retVal = (*rit)->update(event, ref, set)) != 0)
	  return retVal;
    }
  for (auto rit = _widgets.rbegin(); rit != _widgets.rend(); ++rit)
    {
      if ((retVal = (*rit)->update(event, ref, set)) != 0)
	return retVal;
    }
  return 0;
}

int		GamePanel::updateNetwork(Settings &set)
{
  ENetEvent	event;

  try
    {
      if (_socket.pollEvent(&event, 1) < 0)
	throw NetworkException("Connection problem");
      switch (event.type)
	{
	case ENET_EVENT_TYPE_CONNECT:
	  connectClient(event.peer, set);
	  break;
	case ENET_EVENT_TYPE_RECEIVE:
	  handlePackets(event);
	  break;
	case ENET_EVENT_TYPE_DISCONNECT:
	  disconnectClient(event.peer);
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

void	GamePanel::handlePackets(ENetEvent &event)
{
  printf ("A packet of length %lu containing %s was received from %s on channel %u.\n",
	  event.packet->dataLength,
	  event.packet->data,
	  event.peer->data,
	  event.channelID);
  _proto.parseCmd(event.packet->data, event.packet->dataLength);
  enet_packet_destroy(event.packet);
}

void	GamePanel::connectClient(ENetPeer * const peer, Settings &set)
{
  peer->data = (char *)("Server");
  sendConnectionInfo(set);

  // here query the player's position
  _world->setPlayerPosition({0.5, 0.5});

  // then we could query the chunks

  // +1 is the Center, X * 2 for what is bordering it, + 2 for the sides


  // Vector2f	position = _world.getPlayerPosition();
  // sideSize.x =  1 + (std::stoi(set.getCvarList().getCvar("r_width"))
  // 		     / (Chunk::width * TileCodex::tileSize) * 2) + 2;
  // sideSize.y = 1 + (std::stoi(set.getCvarList().getCvar("r_height"))
  // 		    / (Chunk::height * TileCodex::tileSize) * 2) + 2;
  // for (int y = chunkPos.y - (sideSize.y - 1) / 2;
  //      y <= chunkPos.y + (sideSize.y - 1) / 2; ++y)
  //   {
  //     for (int x = chunkPos.x - (sideSize.x - 1) / 2;
  // 	   x <= chunkPos.x + (sideSize.x - 1) / 2; ++x)
  // 	{
  // 	  _world.loadChunk(clients, x, y);
  // 	  chunks.push_back({x, y});
  // 	}
  //   }
  // client->sendPacket(0, _world.serialize(chunks));
}

void	GamePanel::disconnectClient(ENetPeer * const peer)
{
  std::cout << "Disconnect Peer" << std::endl;
  setHide(true);
  notify(t_event(wEvent::Hide | wEvent::Toggle));
}

void			GamePanel::queryChunks(const std::vector<Vector2i> &chunkIds) const
{
  ClientMessage		msg;
  QueryChunk		*qChunk = new QueryChunk;
  VectorInt		*id;
  std::string		serialized;

  for (auto &chunkId : chunkIds)
    {
      id = qChunk->add_id();
      id->set_x(chunkId.x);
      id->set_y(chunkId.y);
    }
  msg.set_content(ClientMessage::QUERYCHUNK);
  msg.set_allocated_querychunk(qChunk);
  msg.SerializeToString(&serialized);
  _socket.sendPacket(1, serialized);
}

void			GamePanel::sendConnectionInfo(Settings &set) const
{
  ClientMessage		msg;
  ConnectionMessage	*co = new ConnectionMessage;
  std::string		*userId = new std::string;
  std::string		serialized;

  *userId = "toto"; //modify
  co->set_allocated_userid(userId);
  msg.set_content(ClientMessage::CONNECTION);
  msg.set_allocated_co(co);
  msg.SerializeToString(&serialized);

  _socket.sendPacket(1, serialized);
}

int		GamePanel::update(const sf::Event &event,
				  sf::RenderWindow &ref,
				  Settings &set)
{
  int		retVal;

  updateNetwork(set);
  if (_actAnalyzer.getInputChanges(set))
    {
      _socket.sendPacket(1, _actAnalyzer.serialize());
    }
  retVal = updateHud(event, ref, set);
  _world->update();
  return retVal;
}
