#include "SelectList.hpp"

SelectList::SelectList(const sf::FloatRect &zone) :
  ASelectList(zone)
{
  if (!_font.loadFromFile("../client/assets/font.otf"))
    std::cerr << "Can't load font" << std::endl; // replace this by a throw about ressources
}

void	SelectList::construct(const sf::Texture &texture, Settings &set,
			      const std::vector<APanelScreen *> &panels)
{
  Widget	*wHeader = new Widget("header", {_zone.left, _zone.top, _zone.width, 70},
				      sf::Text("Select", _font, 30));

  addSpriteForWidget(wHeader, sf::Color(10, 06, 12, 255), {_zone.width, 50});

  _widgets.push_back(wHeader);
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}
