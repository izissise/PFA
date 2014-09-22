#include "OptionKeyPanel.hpp"
#include "Unused.hpp"

OptionKeyPanel::OptionKeyPanel() :
  APanelScreen()
{
  if (!_font.loadFromFile("../client/assets/font.otf"))
    std::cerr << "Can't load font" << std::endl; // replace this by a throw about ressources
  _hide = true;
}

OptionKeyPanel::~OptionKeyPanel()
{
}

void	OptionKeyPanel::construct(const sf::Texture &texture UNUSED, Settings &set,
			       const std::vector<APanelScreen *> &panels UNUSED)
{
  Widget	*wPanOption = new Widget("panOpt", {380, 120, 1100, 730});
  TextWidget	*wKey = new TextWidget("forward", {450, 300, 25, 25},
				       sf::Text("", _font, 20),
				       sf::Text("", _font, 20), 1);

  createOptPanel(texture, wPanOption);
  createKeyWidget(texture, wKey, Action::ToggleConsole);

  _widgets.push_back(wPanOption);
  _widgets.push_back(wKey);
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	OptionKeyPanel::createOptPanel(const sf::Texture &texture UNUSED, Widget *wPanOption)
{
  addSpriteForWidget(wPanOption, sf::Color(125, 125, 125, 150), {1100, 730});
}

void	OptionKeyPanel::createKeyWidget(const sf::Texture &texture, TextWidget *wKey,
					Action act)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [act](AWidget &widget, const sf::Event &event, sf::RenderWindow &ref UNUSED)
    -> int
    {
      sf::FloatRect zone = widget.getZone();

      widget.alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
      return 0;
    };
  wKey->setFunction("main", updateFunc);
  wKey->addSprite(texture, sf::IntRect(1010, 1085, 25, 25));
  wKey->setColor(sf::Color(0,0,0));
  wKey->setDefaultColor(sf::Color(60,60,60));
  wKey->getCursor().setColor(sf::Color(0,0,0));
}
