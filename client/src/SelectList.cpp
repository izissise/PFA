#include "SelectList.hpp"
#include "ScrollWidget.hpp"

SelectList::SelectList(const sf::FloatRect &zone) :
  ASelectList(zone)
{
  addFont("default", "../client/assets/font.otf");
}

void	SelectList::construct(const sf::Texture &texture, Settings &set,
			      const std::vector<APanelScreen *> &panels UNUSED)
{
  float	contHeight = _zone.height - 60;
  float	contTop = _zone.top + 60;

  Panel		*container = new Panel(sf::FloatRect(_zone.left, contTop,
						     _zone.width, contHeight));
  Widget	*wHeader = new Widget("header", {_zone.left, _zone.top, _zone.width, 60},
				      sf::Text("Select", _font["default"], 25));
  Widget	*wFirst = new Widget("first", {_zone.left, _zone.top + 57, _zone.width, 60},
				      sf::Text("first", _font["default"], 22));
  Widget	*wSecond = new Widget("second", {_zone.left, _zone.top + 114, _zone.width, 60},
				      sf::Text("second", _font["default"], 22));
  Widget	*wThird = new Widget("third", {_zone.left, _zone.top + 171, _zone.width, 60},
				      sf::Text("third", _font["default"], 22));
  Widget	*wFourth = new Widget("fourth", {_zone.left, _zone.top + 228, _zone.width, 60},
				      sf::Text("fourth", _font["default"], 22));
  ScrollWidget	*wScroll = new ScrollWidget("scroll",
					    {_zone.left + _zone.width - 13, contTop, 13, contHeight},
					    Scroll::Vertical, container,
					    sf::Text(), wFlag::None);

  createHeader(texture, wHeader);
  createButton(texture, wFirst);
  createButton(texture, wSecond);
  createButton(texture, wThird);
  createButton(texture, wFourth);
  createScroll(texture, wScroll);

  wHeader->addObserver(container);
  addObserver(container);

  _widgets.push_back(wHeader);
  container->addWidget({wFirst, wSecond, wThird, wFourth, wScroll});
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

void	SelectList::createScroll(const sf::Texture &texture, ScrollWidget *wScroll)
{
  wScroll->addSprite(texture, sf::IntRect(1012, 1085, 13, 13));
  wScroll->addSprite(texture, sf::IntRect(1025, 1085, 13, 13));
  wScroll->addSprite(texture, sf::IntRect(1038, 1085, 13, 13));
  wScroll->toSize(0, 13, _zone.height - 60);
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
	      widget.notify(t_event(wEvent::UpdateText, 0, 0,
				    widget.getTextContent().toAnsiString()));
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
