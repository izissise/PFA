#include "ServerPanel.hpp"

ServerPanel::ServerPanel(const sf::FloatRect &zone,
			 const std::string &ip) :
  APanelScreen(zone), _ip(ip);
{
  addFont("default", "../client/assets/default.TTF");
}

void	ServerPanel::construct(const sf::Texture &texture, Settings &set,
			       const std::vector<APanelScreen *> &panels)
{
  Widget	*wName = new Widget("Name", {_zone.left, _zone.top, 25.f * _zone.width / 100.f, 30},
				    sf::Text("Server's Name", _font["default"], 20));
  Widget	*wCountry = new Widget("Country", {_zone.left + 25.f * _zone.width / 100.f, _zone.top,
						5.f * _zone.width / 100.f, 30}
						sf::Text("NA", _font["default"], 20));
  Widget	*wPlayers = new Widget("Players", {_zone.left + 30.f * _zone.width / 100.f, _zone.top,
					10.f * _zone.width / 100.f, 30},
					sf::Text("nb/max", _font["default"], 20));
  Widget	*wPing = new Widget("Ping", {_zone.left + 40.f * _zone.width / 100.f, _zone.top,
					10.f * _zone.width / 100.f, 30},
					sf::Text("nb ms", _font["default"], 20));
  unsigned int	id = panels[0].getSubPanels().size();

  createNameWidget(wName);
  createCountryWidget(wCountry);
  createPlayerWidget(wPlayers);
  createPingWidget(wPing);
  applyButtonStyle(wName, id);
  applyButtonStyle(wCountry, id);
  applyButtonStyle(wPlayers, id);
  applyButtonStyle(wPing, id);
  addWidget{wName, wCountry, wPlayers, wPing};
}

void	ServerPanel::createNameWidget(Widget *widget)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      return 0;
    };
  createButtonStyle(widget, texture);
  widget->setUpdate(updateFunc);
}

void	ServerPanel::createCountryWidget(Widget *widget)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      return 0;
    };
  createButtonStyle(widget, texture);
  widget->setUpdate(updateFunc);
}

void	ServerPanel::createPlayerWidget(Widget *widget)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      return 0;
    };
  createButtonStyle(widget, texture);
  widget->setUpdate(updateFunc);
}

void	ServerPanel::createPingWidget(Widget *widget)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      return 0;
    };
  createButtonStyle(widget, texture);
  widget->setUpdate(updateFunc);
}

void	ServerPanel::applyButtonStyle(Widget *widget, unsigned int id);
{
  sf::FloatRect zone = widget->getZone();

  zone->alignText({zone.left, zone.top}, {zone.width, 100}, 50, 50);
}
