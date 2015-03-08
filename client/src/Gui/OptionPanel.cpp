#include "OptionPanel.hpp"
#include "Unused.hpp"

OptionPanel::OptionPanel(const sf::FloatRect &zone) :
  APanelScreen(zone)
{
}

OptionPanel::~OptionPanel()
{
}

void	OptionPanel::construct(const sf::Texture &texture UNUSED, Settings &set UNUSED,
				  const std::vector<APanelScreen *> &panels UNUSED)
{
}

void	OptionPanel::trigger(const t_event &ev)
{
  if (ev.e & (wEvent::Hide | wEvent::Toggle))
    {
      _hide = !_hide;
      notify(t_event(wEvent::Hide, 0, 1));
    }
}
