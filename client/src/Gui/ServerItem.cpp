#include "ServerItem.hpp"

ServerItem::ServerItem(const sf::FloatRect &zone, const std::string &ip) :
  APanelScreen(zone), _ip(ip)
{
  addFont("default", "../client/assets/default.TTF");
}

void	ServerItem::construct(const sf::Texture &texture, Settings &set,
			      const std::vector<APanelScreen *> &panels)
{
  Widget	*wBg = new Widget("", _zone);
  Widget	*wName = new Widget("Name", sf::FloatRect(_zone.left, _zone.top,
							  600, _zone.height),
				    sf::Text("", _font["default"], 20));
  Widget	*wCountry = new Widget("Country", sf::FloatRect(_zone.left + 600, _zone.top,
								50, _zone.height),
				       sf::Text("", _font["default"], 20));
  Widget	*wPlayers = new Widget("Players", sf::FloatRect(_zone.left + 650, _zone.top,
								100, _zone.height),
				       sf::Text("", _font["default"], 20));
  Widget	*wPing = new Widget("Ping", sf::FloatRect(_zone.left + 750, _zone.top,
							  50, _zone.height),
				    sf::Text("", _font["default"], 20));

  createBackgroundController(wBg);
  wBg->addObserver(panels[0]);
  addWidget({wBg, wName, wCountry, wPlayers, wPing});
  updateItem({"Long server name", "FR", "20/60", "35"}); // as an example
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	ServerItem::createBackgroundController(Widget *widget)
{
  addSpriteForWidget(widget, sf::Color(59, 63, 62, 255), {_zone.width, _zone.height});
  addSpriteForWidget(widget, sf::Color(91, 111, 58, 255), {_zone.width, _zone.height});
  widget->setSpriteAttr(1, false);
  widget->setEdge(sf::Vector2f(_zone.width, _zone.height), 3.f,
	       sf::Color(46, 50, 49, 255));

  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateDisplay =
    [this](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(event, sf::Mouse::Right))
	    {
	      t_event	evt(wEvent::Update | wEvent::Hide | wEvent::Toggle);

	      evt.str = getWidget("Name")->getContent() + "\n" +
	      	getIp() + "\n" +
		getWidget("Ping")->getContent() + "\n" +
	      	getWidget("Players")->getContent();
	      lwidget.notify(evt);
	      return 1;
	    }
	  else if (lwidget.isClicked(event, sf::Mouse::Left))
	    {
	      t_event	evt(wEvent::Update | wEvent::Show);

	      evt.str = getWidget("Name")->getContent() + "\n" +
	      	getIp() + "\n" +
		getWidget("Ping")->getContent() + "\n" +
	      	getWidget("Players")->getContent();
	      lwidget.notify(evt);
	      return 1;
	    }
	}
      return 0;
    };
  widget->setUpdate(updateDisplay);
}

void	ServerItem::updateItem(const std::vector<std::string> &info)
{
  std::vector<AWidget *>	widgets {getWidget("Name"), getWidget("Country"),
      getWidget("Players"), getWidget("Ping")};
  for (unsigned int i = 0; i < widgets.size(); ++i)
    {
      const sf::FloatRect &zone = widgets[i]->getZone();
      AWidget	*widget = widgets.at(i);

      widget->setTextContent(info.at(i));
      if (i == 0)
	widget->alignTextLeft({zone.left, zone.top}, {zone.width, zone.height}, 3, 50);
      else
	widget->alignText({zone.left, zone.top}, {zone.width, zone.height}, 50, 50);
    }
}

const std::string	&ServerItem::getIp() const
{
  return _ip;
}
