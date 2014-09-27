#include "OptionGamePanel.hpp"
#include "Unused.hpp"

OptionGamePanel::OptionGamePanel() :
  APanelScreen()
{
  if (!_font.loadFromFile("../client/assets/font.otf"))
    std::cerr << "Can't load font" << std::endl; // replace this by a throw about ressources
}

OptionGamePanel::~OptionGamePanel()
{
}

void	OptionGamePanel::construct(const sf::Texture &texture UNUSED, Settings &set,
			       const std::vector<APanelScreen *> &panels UNUSED)
{
  Widget	*wPanOption = new Widget("panOpt", {380, 120, 1100, 730},
					 sf::Text(), wFlag::None);
  Widget	*wCheckBox = new Widget("checkBox", {450, 200, 50, 50});
  TextWidget	*wTextWidget = new TextWidget("nameText", {450, 300, 300, 40},
					      sf::Text("", _font, 20),
					      sf::Text("Name", _font, 20), 30);
  BarWidget	*wBarWidget = new BarWidget("bar", {450, 400, 550, 10});
  ScrollWidget	*wScrollWidget = new ScrollWidget("scroll", {1480, 120, 20, 730},
						  Scroll::Vertical, this,
						  sf::Text(), wFlag::None);
  SelectList	*wSelectList = new SelectList({600, 1000, 200, 600});

  wSelectList->construct(texture, set, {});
  createOptPanel(texture, wPanOption);
  createCheckBox(texture, wCheckBox);
  createTextWidget(texture, wTextWidget);

  wBarWidget->addSprite(texture, sf::IntRect(970, 1080, 549, 5));
  wBarWidget->resize(0.5, 1.0);
  wBarWidget->addSprite(texture, sf::IntRect(990, 1085, 20, 21));

  wScrollWidget->addSprite(texture, sf::IntRect(670, 1120, 800, 20));
  wScrollWidget->addSprite(texture, sf::IntRect(1011, 1085, 18, 18));
  wScrollWidget->toSize(1, 15, 15);
  wScrollWidget->toSize(0, 730, 17);
  wScrollWidget->getSprite(0).sprite.rotate(90);

  _widgets.push_back(wPanOption);
  _widgets.push_back(wScrollWidget);
  _widgets.push_back(wCheckBox);
  _widgets.push_back(wTextWidget);
  _widgets.push_back(wBarWidget);
  addPanels({wSelectList});
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
  	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	OptionGamePanel::createOptPanel(const sf::Texture &texture UNUSED, Widget *wPanOption)
{
  addSpriteForWidget(wPanOption, sf::Color(125, 125, 125, 150), {1100, 730});
}

void	OptionGamePanel::createCheckBox(const sf::Texture &texture, Widget *wCheckBox)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = widget.isOver(ref);
      widget.setSpriteAttr(0, !isOver);
      widget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (widget.isClicked(event, sf::Mouse::Left))
	    {
	      widget.toggleSpriteAttr(2);
	      return 1;
	    }
	}
      return 0;
    };
  wCheckBox->addSprite(texture, sf::IntRect(520, 1080, 50, 50));
  wCheckBox->addSprite(texture, sf::IntRect(570, 1080, 50, 50), false);
  wCheckBox->addSprite(texture, sf::IntRect(620, 1080, 50, 50), false);
  // wCheckBox->setSpriteSize(2, 50, 50);
  wCheckBox->setFunction("main", updateFunc);
  wCheckBox->resize(0.5, 0.5);
}

void	OptionGamePanel::createTextWidget(const sf::Texture &texture UNUSED, TextWidget *wTextWidget)
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
  wTextWidget->addSprite(texture, sf::IntRect(670, 1080, 300, 40));
  wTextWidget->setFunction("main", updateFunc);
  wTextWidget->setColor(sf::Color(0,0,0));
  wTextWidget->setDefaultColor(sf::Color(60,60,60));
  wTextWidget->getCursor().setColor(sf::Color(0,0,0));
}
