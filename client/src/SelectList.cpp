#include "SelectList.hpp"

SelectList::SelectList(const sf::FloatRect &zone) :
  ASelectList(zone)
{
  if (!_font.loadFromFile("../client/assets/font.otf"))
    std::cerr << "Can't load font" << std::endl; // replace this by a throw about ressources
}

void	SelectList::construct(const sf::Texture &texture, Settings &set,
			      const std::vector<APanelScreen *> &panels UNUSED)
{
  Widget	*wHeader = new Widget("header", {_zone.left, _zone.top, _zone.width, 60},
				      sf::Text("Select", _font, 25));
  Widget	*wFirst = new Widget("first", {_zone.left, _zone.top + 57, _zone.width, 60},
				      sf::Text("first", _font, 22));
  Widget	*wSecond = new Widget("second", {_zone.left, _zone.top + 114, _zone.width, 60},
				      sf::Text("second", _font, 22));
  Widget	*wThird = new Widget("third", {_zone.left, _zone.top + 171, _zone.width, 60},
				      sf::Text("third", _font, 22));


  createHeader(texture, wHeader);
  createButton(texture, wFirst);
  createButton(texture, wSecond);
  createButton(texture, wThird);

  wHeader->addObserver({wFirst, wSecond, wThird});
  wFirst->addObserver({wSecond, wThird});
  wSecond->addObserver({wFirst, wThird});
  wThird->addObserver({wSecond, wFirst});
  _widgets.push_back(wHeader);
  _widgets.push_back(wFirst);
  _widgets.push_back(wSecond);
  _widgets.push_back(wThird);
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	SelectList::createHeader(const sf::Texture &texture UNUSED, Widget *w)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;
  sf::FloatRect	wZone = w->getZone();

  updateFunc = [](AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;

      isOver = widget.isOver(ref);
      if (isOver && widget.isClicked(event, sf::Mouse::Left))
	{
	  widget.notify(t_event(wEvent::Toggle | wEvent::Hide));
	  return 1;
	}
      return 0;
    };
  w->setFunction("main", updateFunc);
  addSpriteForWidget(w, sf::Color(10, 06, 12, 255), {_zone.width, 60});
  w->alignText({wZone.left,wZone.top}, {wZone.width,wZone.height}, 30, 50);
}

void	SelectList::createButton(const sf::Texture &texture, Widget *w)
{
  std::function	<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
    updateFunc;
  sf::FloatRect	wZone = w->getZone();

  updateFunc = [](AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      bool	isOver;
      bool	active;

      isOver = widget.isOver(ref);
      if (isOver && widget.isClicked(event, sf::Mouse::Left))
	{
	  active = widget.getSpriteAttr(1).draw;
	  if (!active)
	    {
	      widget.notify(t_event(wEvent::SetSprite, 0, 1));
	      widget.notify(t_event(wEvent::SetSprite, 1, 0));
	    }
	  widget.toggleSpriteAttr(0);
	  widget.toggleSpriteAttr(1);
	  return 1;
	}
      return 0;
    };
  w->setFunction("main", updateFunc);
  w->addSprite(texture, sf::IntRect(0, 1080, 260, 60));
  w->addSprite(texture, sf::IntRect(260, 1080, 260, 60), false);
  w->alignText({wZone.left,wZone.top}, {wZone.width,wZone.height}, 50, 50);
  w->setHidden(true);
}
