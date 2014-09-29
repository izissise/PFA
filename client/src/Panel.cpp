#include "Panel.hpp"

Panel::Panel() : APanelScreen()
{
}

Panel::~Panel()
{
}

void		Panel::construct(const sf::Texture &texture UNUSED, Settings &set UNUSED,
				 const std::vector<APanelScreen *> &panels UNUSED)
{
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

