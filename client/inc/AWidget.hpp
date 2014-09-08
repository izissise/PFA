#ifndef _AWIDGET_H_
#define _AWIDGET_H_

#include <SFML/Graphics.hpp>
#include <map>
#include <functional>
#include <list>
#include "Observer.hpp"
#include "Settings.hpp"

#define SIZEX 1600.0
#define SIZEY 900.0

class AWidget : public IObserver, public Observable
{
public:
  enum	wAction
    {
      None = 0,
      Handled = 1,
      ChangePanel = 2
    };
inline int	operator()(AWidget::wAction a)
{
  return static_cast<int>(a);
};

public:
  AWidget(const std::string &id, const sf::FloatRect &zone,
	  const sf::Text &text);
  virtual ~AWidget();

  virtual int		update(const sf::Event &event, sf::RenderWindow &ref, Settings &set) = 0;
  virtual void		draw(sf::RenderWindow &window) const;

  void		addSprite(sf::Sprite &sprite);
  void		addSprite(const sf::Texture &texture, const sf::IntRect &rect);
  void		alignText(const sf::Vector2f &pos, const sf::Vector2f &size,
			  float xPercent, float yPercent);
  void		alignTextLeft(const sf::Vector2f &pos, const sf::Vector2f &size,
			      float xPercent, float yPercent);
  void		setTextPosition(int x, int y);
  void		setTextAttr(unsigned int style);
  void		setHidden(bool state);
  void		setFunction(const std::string &key, const std::function
			    <int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
			    &func);
  void		resize(const sf::Vector2f &size);
  void		trigger(const std::string &event);

public:		// public so the lambda can call it
  bool		isOver(const sf::RenderWindow &ref) const;
  bool		isClicked(const sf::Event &event, sf::Mouse::Button button) const;
  void		eOver(unsigned int spritePos);

protected:
  const std::string		_id;
  sf::FloatRect			_zone;
  int				_spritePos;
  std::vector<sf::Sprite>	_sprites;
  sf::Text			_text;
  bool				_hidden;
  std::map<std::string, std::function
	   <int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>>
    _updates;
};

inline int	operator&(AWidget::wAction a, AWidget::wAction b)
{
  return (static_cast<int>(a) & static_cast<int>(b));
};

#endif /* _AWIDGET_H_ */
