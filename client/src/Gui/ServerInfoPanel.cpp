#include "ServerInfoPanel.hpp"

ServerInfoPanel::ServerInfoPanel(const sf::FloatRect &zone,
				 const std::string &ip) :
  APanelScreen(zone), _ip(ip)
{
  addFont("default", "../client/assets/default.TTF");
}

void	ServerInfoPanel::construct(const sf::Texture &texture UNUSED, Settings &set UNUSED,
				   const std::vector<APanelScreen *> &panels UNUSED)
{
  Widget	*wName = new Widget("Name", {_zone.left, _zone.top,
						25.f * _zone.width / 100.f, _zone.height},
				    sf::Text("Server's Name", _font["default"], 20));
  Widget	*wCountry = new Widget("Country", {_zone.left + 25.f * _zone.width / 100.f, _zone.top,
						5.f * _zone.width / 100.f, _zone.height},
						sf::Text("NA", _font["default"], 20));
  Widget	*wPlayers = new Widget("Players", {_zone.left + 30.f * _zone.width / 100.f, _zone.top,
					10.f * _zone.width / 100.f, _zone.height},
					sf::Text("nb/max", _font["default"], 20));
  Widget	*wPing = new Widget("Ping", {_zone.left + 40.f * _zone.width / 100.f, _zone.top,
					10.f * _zone.width / 100.f, _zone.height},
					sf::Text("nb ms", _font["default"], 20));

  createNameWidget(wName);
  createCountryWidget(wCountry);
  createPlayerWidget(wPlayers);
  createPingWidget(wPing);
  applyButtonStyle(wName);
  applyButtonStyle(wCountry);
  applyButtonStyle(wPlayers);
  applyButtonStyle(wPing);
  addWidget({wName, wCountry, wPlayers, wPing});
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	ServerInfoPanel::createNameWidget(Widget *widget)
{
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](UNUSED AWidget &lwidget, UNUSED const sf::Event &ev, UNUSED sf::RenderWindow &ref)
    -> int
    {
      return 0;
    };
  widget->setUpdate(updateFunc);
}

void	ServerInfoPanel::createCountryWidget(Widget *widget)
{
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](UNUSED AWidget &lwidget, UNUSED const sf::Event &ev, UNUSED sf::RenderWindow &ref)
    -> int
    {
      return 0;
    };
  widget->setUpdate(updateFunc);
}

void	ServerInfoPanel::createPlayerWidget(Widget *widget)
{
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](UNUSED AWidget &lwidget, UNUSED const sf::Event &ev, UNUSED sf::RenderWindow &ref)
    -> int
    {
      return 0;
    };
  widget->setUpdate(updateFunc);
}

void	ServerInfoPanel::createPingWidget(Widget *widget)
{
  std::function	<int (AWidget &widget, const sf::Event &ev, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](UNUSED AWidget &lwidget, UNUSED const sf::Event &ev, UNUSED sf::RenderWindow &ref)
    -> int
    {
      return 0;
    };
  widget->setUpdate(updateFunc);
}

void	ServerInfoPanel::applyButtonStyle(Widget *widget)
{
  sf::FloatRect zone = widget->getZone();

  widget->alignText({zone.left, zone.top}, {zone.width, zone.height}, 50, 50);
}

const std::string	&ServerInfoPanel::getIp() const
{
  return _ip;
}
