#include "CheckBoxWidget.hpp"

CheckBoxWidget::CheckBoxWidget(const std::string &id, const sf::FloatRect &zone,
		   const sf::Text &text, wFlag flg) :
  AWidget(id, zone, text, flg)
{
}

int		CheckBoxWidget::update(const sf::Event &event, sf::RenderWindow &ref,
				       Settings &set UNUSED)
{
  bool		over;
  int		retVal = 0;

  over = isOver(ref);
  setSpriteAttr(0, !over);
  setSpriteAttr(1, over);
  if (over)
    {
      if (isClicked(event, sf::Mouse::Left))
	{
	  toggleSpriteAttr(2);
	  return 1;
	}
    }
  if (_update)
    if (_update(*this, event, ref) != 0)
      return 1;
  return retVal;
}
