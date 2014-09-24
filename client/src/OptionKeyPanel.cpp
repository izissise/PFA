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
  KeyWidget	*wForward = new KeyWidget("test", {450, 400, 150, 40},
				       Action::Forward, set.getControls(),
				       sf::Text("", _font, 20));

  createOptPanel(texture, wPanOption);
  createKeyWidget(texture, wForward);

  _widgets.push_back(wPanOption);
  _widgets.push_back(wForward);
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	OptionKeyPanel::createOptPanel(const sf::Texture &texture UNUSED, Widget *wPanOption)
{
  addSpriteForWidget(wPanOption, sf::Color(125, 125, 125, 150), {1100, 730});
}

void	OptionKeyPanel::createKeyWidget(const sf::Texture &texture UNUSED, KeyWidget *wKey)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &widget, const sf::Event &event UNUSED, sf::RenderWindow &ref UNUSED)
    -> int
    {
      sf::FloatRect zone = widget.getZone();

      widget.alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
      return 0;
    };
  wKey->addSprite(texture, sf::IntRect(670, 1080, 300, 40));
  wKey->toSize(0, 150, 40);
  wKey->setFunction("main", updateFunc);
  wKey->setColor(sf::Color(0,0,0));
  //  wKey->setTextAttr(sf::Text::Bold);
}
