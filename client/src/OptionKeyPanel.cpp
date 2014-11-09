#include "OptionKeyPanel.hpp"
#include "Unused.hpp"

OptionKeyPanel::OptionKeyPanel(const sf::FloatRect &zone) :
  APanelScreen(zone)
{
  addFont("default", "../client/assets/font.otf");
  addFont("Title", "../client/assets/Title-font.ttf");
  _hide = true;
}

OptionKeyPanel::~OptionKeyPanel()
{
}

void	OptionKeyPanel::construct(const sf::Texture &texture UNUSED, Settings &set,
			       const std::vector<APanelScreen *> &panels UNUSED)
{
  sf::Text	txt = sf::Text("", _font["default"], 20);

  Controls	&ctrl = set.getControls();

  Widget	*wPanOption = new Widget("panOpt", {380, _zone.top, 1100, _zone.height},
					 sf::Text(), wFlag::Resizable);
  ScrollWidget	*wScroll = new ScrollWidget("scroll", {1480, _zone.top, 13, _zone.height},
					    Scroll::Vertical, this,
					    sf::Text(), wFlag::None);
  Widget	*wTitleKey = new Widget("TitleWidget", {_zone.left + 18, _zone.top + 18, 200, 40},
					sf::Text("GAME CONTROLS", _font["Title"], 29));
  KeyWidget	*wForward = new KeyWidget("Forward", {450, 300, 150, 40}, Action::Forward, ctrl, txt);
  KeyWidget	*wBack = new KeyWidget("Back", {450, 400, 150, 40}, Action::Back, ctrl, txt);
  KeyWidget	*wRight = new KeyWidget("Right", {450, 500, 150, 40}, Action::Right, ctrl, txt);
  KeyWidget	*wLeft = new KeyWidget("Left", {450, 600, 150, 40}, Action::Left, ctrl, txt);
  KeyWidget	*wUse = new KeyWidget("Use", {450, 700, 150, 40}, Action::Use, ctrl, txt);
  KeyWidget	*wMoveUp = new KeyWidget("moveup", {450, 800, 150, 40}, Action::MoveUp, ctrl, txt);
  KeyWidget	*wMoveDown = new KeyWidget("movedown", {450, 900, 150, 40},
					   Action::MoveDown, ctrl, txt);
  KeyWidget	*wConsole = new KeyWidget("ToggleConsole", {450, 1000, 150, 40},
						Action::ToggleConsole, ctrl, txt);
  Widget	*wTforward = new Widget("Tforward", {600, 300, 150, 40},
					sf::Text("Forward", _font["default"], 20));
  Widget	*wTback = new Widget("Tback", {600, 400, 150, 40},
				     sf::Text("Back", _font["default"], 20));
  Widget	*wTright = new Widget("Tright", {600, 500, 150, 40},
				      sf::Text("Right", _font["default"], 20));
  Widget	*wTleft = new Widget("Tleft", {600, 600, 150, 40},
				     sf::Text("Left", _font["default"], 20));
  Widget	*wTuse = new Widget("Tuse", {600, 700, 150, 40},
				    sf::Text("Use", _font["default"], 20));
  Widget	*wTconsole = new Widget("Tconsole", {600, 800, 150, 40},
					sf::Text("Console", _font["default"], 20));


  createOptPanel(texture, wPanOption);
  createScrollBar(texture, wScroll);
  createKeyWidget(texture, wForward);
  createKeyWidget(texture, wBack);
  createKeyWidget(texture, wRight);
  createKeyWidget(texture, wLeft);
  createKeyWidget(texture, wUse);
  createKeyWidget(texture, wMoveUp);
  createKeyWidget(texture, wMoveDown);
  createKeyWidget(texture, wConsole);
  createTextWidget(wTforward);
  createTextWidget(wTback);
  createTextWidget(wTright);
  createTextWidget(wTleft);
  createTextWidget(wTuse);
  createTextWidget(wTconsole);

  _widgets.push_back(wPanOption);
  _widgets.push_back(wScroll);
  _widgets.push_back(wTitleKey);
  _widgets.push_back(wForward);
  _widgets.push_back(wBack);
  _widgets.push_back(wRight);
  _widgets.push_back(wLeft);
  _widgets.push_back(wUse);
  _widgets.push_back(wMoveUp);
  _widgets.push_back(wMoveDown);
  _widgets.push_back(wConsole);
  _widgets.push_back(wTforward);
  _widgets.push_back(wTback);
  _widgets.push_back(wTright);
  _widgets.push_back(wTleft);
  _widgets.push_back(wTuse);
  _widgets.push_back(wTconsole);
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	OptionKeyPanel::createOptPanel(const sf::Texture &texture UNUSED, Widget *wPanOption)
{
  addSpriteForWidget(wPanOption, sf::Color(125, 125, 125, 150), {1100, 730});
}

void	OptionKeyPanel::createKeyWidget(const sf::Texture &texture UNUSED, KeyWidget *wKey)
{
  sf::FloatRect wzone = wKey->getZone();
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &widget, const sf::Event &event UNUSED, sf::RenderWindow &ref UNUSED)
    -> int
    {
      sf::FloatRect zone = widget.getZone();

      widget.alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
      return 0;
    };
  wKey->alignText({wzone.left,wzone.top}, {wzone.width, wzone.height}, 50, 50);
  wKey->addSprite(texture, sf::IntRect(670, 1080, 300, 40));
  wKey->setSpriteSize(0, 150, 40);
  wKey->setUpdate(updateFunc);
  wKey->setColor(sf::Color(0,0,0));
  //  wKey->setTextAttr(sf::Text::Bold);
}

void	OptionKeyPanel::createScrollBar(const sf::Texture &texture UNUSED, ScrollWidget *wScroll)
{
  wScroll->addSprite(texture, sf::IntRect(1012, 1085, 13, 13));
  wScroll->addSprite(texture, sf::IntRect(1025, 1085, 13, 13));
  wScroll->addSprite(texture, sf::IntRect(1038, 1085, 13, 13));
  wScroll->setSpriteSize(0, 13, 730);
}

void	OptionKeyPanel::createTextWidget(Widget *widget)
{
  sf::FloatRect zone = widget->getZone();

  widget->alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 45);
}
