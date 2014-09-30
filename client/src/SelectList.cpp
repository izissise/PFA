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
  Panel		*container = new Panel();
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

  wHeader->addObserver(container);
  addObserver(container);

  _widgets.push_back(wHeader);
  container->addWidget({wFirst, wSecond, wThird});
  container->construct(texture, set, panels);
  container->setHide(true);
  addPanels({container});
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	SelectList::trigger(const t_event &event)
{
  if (event.e & wEvent::Hide)
    {
      if (event.e & wEvent::Toggle)
	_hide = !_hide;
      else
	{
	  _hide = event.value;
	}
    }
  else if (event.e & wEvent::UpdateText)
    {
      _widgets[0]->setTextContent(event.strText); // widget[0] is the Header
      notify(t_event(wEvent::Hide));
    }
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
  w->setUpdate(updateFunc);
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

      isOver = widget.isOver(ref);
      widget.setSpriteAttr(0, !isOver);
      widget.setSpriteAttr(1, isOver);
      if (isOver)
	{
	  if (widget.isClicked(event, sf::Mouse::Left))
	    {
	      widget.notify(t_event(wEvent::UpdateText, 0, 0, widget.getTextContent()));
	      return 1;
	    }
	}
      return 0;
    };
  w->setUpdate(updateFunc);
  w->addObserver(this);
  w->addSprite(texture, sf::IntRect(0, 1080, 260, 60));
  w->addSprite(texture, sf::IntRect(260, 1080, 260, 60), false);
  w->alignText({wZone.left,wZone.top}, {wZone.width,wZone.height}, 50, 50);
}
