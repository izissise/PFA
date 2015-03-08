#include "KeyWidget.hpp"
#include "Exception.hpp"

KeyWidget::KeyWidget(const std::string &id, const sf::FloatRect &zone,
		     Action act, const Controls &ctrl, const sf::Text &text):
  AWidget(id, zone, text), _isActive(false), _action(act), _forceUpdate(false)
{
  getKeyName(ctrl);
}

void	KeyWidget::getKeyName(const Controls &ctrl)
{
  std::array<t_entry, 5> keys = ctrl.getBoundKeys(_action);
  auto end = keys.end();
  std::string	keyNames;

  if (keys.empty() || *(keys.begin()) == ctrl::state::Unset)
    _text.setString("Unset");
  else
    {
      for (auto it = keys.begin(); it != end && !(*it == ctrl::state::Unset); ++it)
	{
	  if (it != keys.begin())
	    keyNames = keyNames + "; ";
	  keyNames = keyNames + ctrl.getCodeFromKey(*it);
	}
      _text.setString(keyNames);
    }
}

void	KeyWidget::bindKey(Settings &set)
{
  Controls	&ctrl = set.getControls();

  if (ctrl.isKnownKey(_entry))
    ctrl.bindKeyOnAction(_entry, _action);
  getKeyName(ctrl);
}

void	KeyWidget::unbindKey(Settings &set)
{
  Controls	&ctrl = set.getControls();

  ctrl.unbindKeyFromAction(_entry, _action);
  getKeyName(ctrl);
}

int	KeyWidget::update(const sf::Event &event, sf::RenderWindow &ref, Settings &set)
{
  int	retVal = 0;

  BlinkText::update();
  if (isClicked(event, sf::Mouse::Left) && !_isActive)
    {
      _isActive = isOver(ref);
      if (_isActive)
	{
	  retVal = _isActive;
	  _entry.key = sf::Keyboard::Unknown;
	  // _text.setString("");
	}
    }
  else if (_isActive)
    {
      if (event.type == sf::Event::KeyPressed ||
	  event.type == sf::Event::MouseButtonPressed ||
	  event.type == sf::Event::MouseWheelMoved)
	{
	  std::array<t_entry, 5> keys = set.getControls().getBoundKeys(_action);
	  _entry.fill(event);
	  _isActive = false;
	  retVal = 1;
	  if (std::find(keys.begin(), keys.end(), _entry) != keys.end())
	    unbindKey(set);
	  else
	    bindKey(set);
	  if (_update)
	    _update(*this, event, ref);
	}
    }
  else if (_forceUpdate)
    {
      _forceUpdate = false;
      getKeyName(set.getControls());
      if (_update)
	_update(*this, event, ref);
    }
  return retVal;
}

void	KeyWidget::draw(sf::RenderTexture &window) const
{
  sf::Shape	*edge = _edge.get();

  for (auto &elem : _sprites)
    if (elem.draw)
      window.draw(elem.sprite);
  if (edge)
    window.draw(*edge);
  if (_isActive)
    {
      if (getDisplayState())
	window.draw(_text);
    }
  else
    window.draw(_text);
}

void	KeyWidget::trigger(const t_event &event)
{
  if (event.e & wEvent::Update)
    _forceUpdate = true;
  else
    AWidget::trigger(event);
}
