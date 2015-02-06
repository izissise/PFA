#include "OptionGamePanel.hpp"
#include "Unused.hpp"

OptionGamePanel::OptionGamePanel(const sf::FloatRect &zone) :
  APanelScreen(zone)
{
  addFont("default", "../client/assets/default.TTF");
}

OptionGamePanel::~OptionGamePanel()
{
}

void	OptionGamePanel::construct(const sf::Texture &texture UNUSED, Settings &set,
			       const std::vector<APanelScreen *> &panels UNUSED)
{
  Widget	*wPanOption = new Widget("panOpt", {_zone.left, _zone.top, _zone.width, _zone.height},
					 sf::Text(), wFlag::None);
  CheckBoxWidget *wCheckBox = new CheckBoxWidget("checkBox", {450, 200, 50, 50});
  TextWidget	*wTextWidget = new TextWidget("nameText", {450, 700, 300, 40},
					      sf::Text("", _font["default"], 20),
					      sf::Text("Name", _font["default"], 20), 30);
  BarWidget	*wBarWidget = new BarWidget("bar", {450, 400, 550, 10});
  ScrollWidget	*wScroll = new ScrollWidget("scroll", {_zone.left + _zone.width - 13, _zone.top, 13, _zone.height},
					    Scroll::Vertical, this,
					    sf::Text(), wFlag::None);
  SelectList	*wSelectList = new SelectList(sf::FloatRect{1000, 200, 260, 231});
  SwitchWidget	*pSwitch = new SwitchWidget(sf::FloatRect{1000, 600, 260, 50}, {1050, 600, 160, 50});

  createOptPanel(texture, wPanOption);
  createCheckBox(texture, wCheckBox);
  createTextWidget(texture, wTextWidget);
  createScrollWidget(texture, wScroll);
  createSwitchWidget(texture, pSwitch);

  wBarWidget->addSprite(texture, sf::IntRect(970, 1080, 549, 5));
  wBarWidget->resize(0.5, 1.0);

  wBarWidget->addSprite(texture, sf::IntRect(990, 1085, 20, 21));

  _widgets.push_back(wPanOption);
  _widgets.push_back(wScroll);
  _widgets.push_back(wCheckBox);
  _widgets.push_back(wTextWidget);
  _widgets.push_back(wBarWidget);
  wSelectList->construct(texture, set, {});
  pSwitch->construct(texture, set, {});
  addPanel({wSelectList,pSwitch});
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
  	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	OptionGamePanel::createOptPanel(const sf::Texture &texture UNUSED, Widget *wPanOption)
{
  addSpriteForWidget(wPanOption, sf::Color(125, 125, 125, 100), {_zone.width, _zone.height});
}

void	OptionGamePanel::createCheckBox(const sf::Texture &texture,
					CheckBoxWidget *wCheckBox)
{
  wCheckBox->addSprite(texture, sf::IntRect(520, 1080, 50, 50));
  wCheckBox->addSprite(texture, sf::IntRect(570, 1080, 50, 50), false);
  wCheckBox->addSprite(texture, sf::IntRect(620, 1080, 50, 50), false);
  wCheckBox->resize(0.5, 0.5);
}

void	OptionGamePanel::createTextWidget(const sf::Texture &texture UNUSED, TextWidget *wTextWidget)
{
  sf::FloatRect	wZone = wTextWidget->getZone();
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
  wTextWidget->setSpriteSize(0, wZone.width, wZone.height);
  wTextWidget->setUpdate(updateFunc);
  wTextWidget->setColor(sf::Color(0,0,0));
  wTextWidget->setDefaultColor(sf::Color(60,60,60));
  wTextWidget->getCursor().setColor(sf::Color(0,0,0));
}

void	OptionGamePanel::createScrollWidget(const sf::Texture &texture, ScrollWidget *wScroll)
{
  wScroll->addSprite(texture, sf::IntRect(1012, 1085, 13, 13));
  wScroll->addSprite(texture, sf::IntRect(1025, 1085, 13, 13));
  wScroll->addSprite(texture, sf::IntRect(1038, 1085, 13, 13));
  wScroll->toSize(0, 13, 730);
}

void	OptionGamePanel::createSwitchWidget(const sf::Texture &texture UNUSED, SwitchWidget *pSwitch)
{
  sf::FloatRect	lZone = pSwitch->getLeftZone();
  sf::FloatRect	rZone = pSwitch->getRightZone();
  sf::FloatRect	cZone = pSwitch->getContentZone();
  Widget	*lSide = new Widget("lSide", lZone, sf::Text("<", _font["default"], 40));
  Widget	*rSide = new Widget("rSide", rZone, sf::Text(">", _font["default"], 40));
  Widget	*content1 = new Widget("c1fezjhkfenzjkfrejlfrjfelzfjeiolfjekmfez", cZone, sf::Text("Low", _font["default"], 20));
  Widget	*content2 = new Widget("c2", cZone, sf::Text("Med", _font["default"], 20));
  Widget	*content3 = new Widget("c3", cZone, sf::Text("High", _font["default"], 20));
  TextWidget	*wTextWidget = new TextWidget("nameText", cZone,
					      sf::Text("", _font["default"], 20),
					      sf::Text("Name", _font["default"], 20), 30);


  pSwitch->addSpriteForWidget(lSide, sf::Color(150,150,150,150), {lZone.width, lZone.height});
  pSwitch->addSpriteForWidget(rSide, sf::Color(150,150,150,150), {rZone.width, rZone.height});
  pSwitch->addSpriteForWidget(content1, sf::Color(0,0,255), {cZone.width, cZone.height});
  pSwitch->addSpriteForWidget(content2, sf::Color(0,255,0), {cZone.width, cZone.height});
  pSwitch->addSpriteForWidget(content3, sf::Color(255,0,0), {cZone.width, cZone.height});
  lSide->alignText({lZone.left, lZone.top}, {lZone.width, lZone.height}, 50, 50);
  rSide->alignText({rZone.left, rZone.top}, {rZone.width, rZone.height}, 50, 50);
  content1->alignText({cZone.left, cZone.top}, {cZone.width, cZone.height}, 50, 50);
  content2->alignText({cZone.left, cZone.top}, {cZone.width, cZone.height}, 50, 50);
  content3->alignText({cZone.left, cZone.top}, {cZone.width, cZone.height}, 50, 50);
  createTextWidget(texture, wTextWidget);
  pSwitch->addSides({lSide, rSide});
  pSwitch->addContent({content1, content2, content3, wTextWidget});
}
