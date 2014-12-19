#include "ServerMenu.hpp"

ServerMenu::ServerMenu(const sf::FloatRect &zone) :
  APanelScreen(zone)
{
  addFont("default", "../client/assets/default.TTF");
  _hide = true;
}

ServerMenu::~ServerMenu()
{
}

void	ServerMenu::construct(const sf::Texture &texture, Settings &set,
			      const std::vector<APanelScreen *> &panels)
{
  Widget	*wTitle = new Widget("title", {_zone.left, _zone.top, _zone.width, 70},
				     sf::Text("Server List", _font["default"], 40));
  Widget	*wFooter = new Widget("Footer", {_zone.left, _zone.height - 70, _zone.width, 70},
				      sf::Text());
  Widget	*wBack = new Widget("Back", {_zone.left + 10, _zone.height - 57, _zone.width / 3, 44},
				    sf::Text("Back to Menu", _font["default"], 20));

  createTitle(wTitle);
  createContPanel(set, texture);
  createFooter(wFooter);
  createButtonBack(wBack, texture);
  wBack->addObserver({this, panels[0]});

  _widgets.push_back(wTitle);
  _widgets.push_back(wFooter);
  _widgets.push_back(wBack);
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	ServerMenu::createContPanel(Settings &set, const sf::Texture &texture)
{
  Panel	*content = new Panel(sf::FloatRect{_zone.left, _zone.top + 70,
	_zone.width, _zone.height - 140});
  sf::FloatRect	contZone = content->getZone();
  Widget	*bgWidget = new Widget("bg", {contZone.left, contZone.top,
				contZone.width, contZone.height}, sf::Text());

  addSpriteForWidget(bgWidget, sf::Color(200, 200, 200, 255), {contZone.width, contZone.height});
  content->addWidget({bgWidget});
  content->construct(texture, set, {});
  addPanels({content});
}

void	ServerMenu::createTitle(Widget *title)
{
  sf::FloatRect zone = title->getZone();

  addSpriteForWidget(title, sf::Color(50, 30, 60, 255), {zone.width, zone.height});
  title->alignText({zone.left, zone.top}, {zone.width, zone.height}, 50, 50);
}

void	ServerMenu::createFooter(Widget *footer)
{
  sf::FloatRect zone = footer->getZone();

  addSpriteForWidget(footer, sf::Color(50, 30, 60, 255), {zone.width, zone.height});
  footer->alignText({zone.left, zone.top}, {zone.width, zone.height}, 50, 50);
}

void	ServerMenu::createButtonBack(Widget *widget, const sf::Texture &texture)
{
  sf::FloatRect zone = widget->getZone();
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;

  updateFunc = [](AWidget &lwidget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = lwidget.isOver(ref);
      lwidget.setSpriteAttr(0, !isOver);
      lwidget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (lwidget.isClicked(event, sf::Mouse::Left))
	    {
	      lwidget.notify(t_event(wEvent::Hide | wEvent::Toggle));
	      return 0;
	    }
	}
      return 0;
    };
  widget->alignText({zone.left,zone.top}, {zone.width, zone.height}, 50, 50);
  widget->addSprite(texture, sf::IntRect(0, 1080, 260, 60));
  widget->addSprite(texture, sf::IntRect(260, 1080, 260, 60), false);
  widget->setSpriteSize(0, zone.width, zone.height);
  widget->setSpriteSize(1, zone.width, zone.height);
  widget->setUpdate(updateFunc);
}
