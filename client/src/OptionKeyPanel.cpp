#include "OptionKeyPanel.hpp"
#include "Unused.hpp"

OptionKeyPanel::OptionKeyPanel(const sf::FloatRect &zone) :
  APanelScreen(zone)
{
  addFont("default", "../client/assets/default.TTF");
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
  sf::FloatRect	contZone = {_zone.left + 18, _zone.top + 200,
			    _zone.width - 36, _zone.height - 250};
  Controls	&ctrl = set.getControls();

  Widget	*wOptionBg = new Widget("panOpt", {_zone.left, _zone.top, _zone.width, _zone.height},
					sf::Text(), wFlag::Resizable);
  Widget	*wTitle = new Widget("TitleWidget", {_zone.left + 18, _zone.top + 18, 200, 40},
				     sf::Text("GAME CONTROLS", _font["Title"], 24));
  Widget	*wDesc = new Widget("Desc", {_zone.left + 18, _zone.top + 50, 200, 40},
					sf::Text("Key mapping for general game controls", _font["default"], 20));
  Widget	*wTabEvent = new Widget("Events", {_zone.left + 18, _zone.top + 150,
					contZone.width / 3, 40},
					sf::Text("Events", _font["default"], 20));
  Widget	*wTabKey = new Widget("Keys", {contZone.left + contZone.width / 3 + 10,
					_zone.top + 150, contZone.width / 3, 40},
					sf::Text("Keys", _font["default"], 20));

  fillOptionBg(texture, wOptionBg);
  _widgets.push_back(wOptionBg);
  _widgets.push_back(wTitle);
  _widgets.push_back(wDesc);
  _widgets.push_back(wTabEvent);
  _widgets.push_back(wTabKey);
  createContPanel(set, texture);

  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	OptionKeyPanel::createEventPanel(Panel *eventPanel, const sf::FloatRect &zone,
					 Settings &set, const sf::Texture &texture)
{
  Widget	*wTextForward = new Widget("textForward", {zone.left, zone.top, zone.width, 40},
					   sf::Text("Move: Forward", _font["default"], 20));
  Widget	*wTextBack = new Widget("textBack", {zone.left, zone.top + 5 * 1 + 40 * 1,
						zone.width, 40},
						sf::Text("Move: Back", _font["default"], 20));
  Widget	*wTextRight = new Widget("textRight", {zone.left, zone.top + 5 * 2 + 40 * 2,
						zone.width, 40},
						sf::Text("Move: Right", _font["default"], 20));
  Widget	*wTextLeft = new Widget("textRight", {zone.left, zone.top + 5 * 15 + 40 * 15,
						zone.width, 40},
						sf::Text("Move: Right", _font["default"], 20));

  createTextWidget(wTextForward);
  createTextWidget(wTextBack);
  createTextWidget(wTextRight);
  createTextWidget(wTextLeft);
  eventPanel->addWidget({wTextForward, wTextBack, wTextRight, wTextLeft});
  eventPanel->construct(texture, set, {});
}

void	OptionKeyPanel::createKeyPanel(Panel *keyPanel, const sf::FloatRect &zone,
				       Settings &set, const sf::Texture &texture)
{
  sf::Text	txt = sf::Text("", _font["default"], 20);
  Controls	&ctrl = set.getControls();

  KeyWidget	*wForward = new KeyWidget("Forward", {zone.left, zone.top, zone.width, 40},
					  Action::Forward, ctrl, txt);
  KeyWidget	*wBack = new KeyWidget("Back", {zone.left, zone.top + 5 * 1 + 40 * 1, zone.width, 40},
					  Action::Back, ctrl, txt);
  KeyWidget	*wRight = new KeyWidget("Right", {zone.left, zone.top + 5 * 2 + 40 * 2, zone.width, 40},
					  Action::Right, ctrl, txt);
  KeyWidget	*wLeft = new KeyWidget("Left", {zone.left, zone.top + 5 * 15 + 40 * 15, zone.width, 40},
					  Action::Left, ctrl, txt);

  createKeyWidget(texture, wForward);
  createKeyWidget(texture, wBack);
  createKeyWidget(texture, wRight);
  createKeyWidget(texture, wLeft);
  keyPanel->addWidget({wForward, wBack, wRight, wLeft});
  keyPanel->construct(texture, set, {});
}

void	OptionKeyPanel::createContPanel(Settings &set, const sf::Texture &texture)
{
  Panel	*contPanel = new Panel({_zone.left + 18, _zone.top + 200,
	_zone.width - 36, _zone.height - 250});
  sf::FloatRect	contZone = contPanel->getZone();

  Widget	*wContBg = new Widget("contBg", {contZone.left, contZone.top,
					contZone.width, contZone.height},
					sf::Text(), wFlag::Resizable);
  ScrollWidget	*wScroll = new ScrollWidget("scroll", {contZone.left + contZone.width - 13,
					contZone.top, 13, contZone.height},
					Scroll::Vertical, contPanel,
					sf::Text(), wFlag::None);

  Panel		*eventPanel = new Panel({contZone.left, contZone.top,
					contZone.width / 3, contZone.height});
  Panel		*keyPanel = new Panel({contZone.left + contZone.width / 3 + 10, contZone.top,
					contZone.width / 3, contZone.height});


  createScrollBar(texture, wScroll);
  contPanel->addSpriteForWidget(wContBg, sf::Color(125, 125, 125, 70),
				{contZone.width, contZone.height}, true);
  createEventPanel(eventPanel, eventPanel->getZone(), set, texture);
  createKeyPanel(keyPanel, keyPanel->getZone(), set, texture);

  contPanel->addWidget({wContBg, wScroll});
  contPanel->addPanels({keyPanel, eventPanel});
  contPanel->construct(texture, set, {});
  addPanels({contPanel});
}

void	OptionKeyPanel::fillOptionBg(const sf::Texture &texture UNUSED, Widget *wPanOption)
{
  addSpriteForWidget(wPanOption, sf::Color(125, 125, 125, 100), {_zone.width, _zone.height});
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
  addSpriteForWidget(wKey, sf::Color(125, 125, 125, 150), {wzone.width, wzone.height});
  wKey->setUpdate(updateFunc);
  //  wKey->setTextAttr(sf::Text::Bold);
}

void	OptionKeyPanel::createScrollBar(const sf::Texture &texture UNUSED, ScrollWidget *wScroll)
{
  sf::FloatRect zone = wScroll->getZone();

  wScroll->addSprite(texture, sf::IntRect(1012, 1085, 13, 13));
  wScroll->addSprite(texture, sf::IntRect(1025, 1085, 13, 13));
  wScroll->addSprite(texture, sf::IntRect(1038, 1085, 13, 13));
  wScroll->setSpriteSize(0, 13, zone.height);
}

void	OptionKeyPanel::createTextWidget(Widget *widget)
{
  sf::FloatRect zone = widget->getZone();

  addSpriteForWidget(widget, sf::Color(125, 125, 125, 150), {zone.width, zone.height});
  widget->alignTextLeft({zone.left,zone.top}, {zone.width, zone.height}, 5, 50);
}
