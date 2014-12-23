#include "Panel.hpp"

Panel::Panel(const sf::FloatRect &zone) :
  APanelScreen(zone)
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

void		Panel::trigger(const t_event &event)
{
  if (_trigger)
    _trigger(event);
  else
    APanelScreen::trigger(event);
}
