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

enum class	wEvent
{
  None = 0,
    Toggle = 1,
    Hide = 2,
    Show = 4,
    SwitchSprite = 8
    };

typedef struct	s_event
{
  wEvent	e;
  int		additional;
  s_event(wEvent event = wEvent::None, int add = 0) :
    e(event), additional(add)
  {
  }
}		t_event;

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

  /**
   * \fn void draw(sf::RenderWindow &window)
   * \brief Draw the widget's sprite and text.
   * \param[in] The render window
   *
   */
  virtual void		draw(sf::RenderWindow &window) const;

  /**
   * \fn void addSprite(sf::Sprite &sprite)
   * \brief Add a sprite to the widget container.
   * \param[in] The sprite to store
   *
   */
  void		addSprite(sf::Sprite &sprite);

  /**
   * \fn void addSprite(const sf::Texture &texture, const sf::IntRect &rect);
   * \brief Add a sprite to the widget container.
   * \param[in] texture: the used texture, rect: the sprite attribute
   *
   * This method is used to not create a local sf::Sprite
   */
  void		addSprite(const sf::Texture &texture, const sf::IntRect &rect);

  /**
   * \fn void alignText(const sf::Vector2f &pos, const sf::Vector2f &size, float xPercent, float yPercent);
   * \brief Align Text from it's center
   * \param[in] pos: Rect(left, top), size: Rect(width, height), xPercent (% width), yPercent (% height)
   *
   * This method let you align a text specifying a zone with pos and size
   * Text will be positioned inside that rectangle so the center will be at
   * xPercent of the width and yPercent of the height rectangle
   */
  void		alignText(const sf::Vector2f &pos, const sf::Vector2f &size,
			  float xPercent, float yPercent);
  /**
   * \fn void alignTextLeft(const sf::Vector2f &pos, const sf::Vector2f &size, float xPercent, float yPercent);
   * \brief Align Text from it's top / left corner
   * \param[in] pos: Rect(left, top), size: Rect(width, height), xPercent (% width), yPercent (% height)
   *
   * This method let you align a text specifying a zone with pos and size
   * Text will be positioned inside that rectangle it's top/left center will be at
   * xPercent of the width and yPercent of the height of the rectangle
   */
  void		alignTextLeft(const sf::Vector2f &pos, const sf::Vector2f &size,
			      float xPercent, float yPercent);
  /**
   * \fn void setTextPosition(int x, int y)
   * \brief Set text position
   * \param[in] x position, y position
   *
   * This method calls sf::Text.setPosition(x, y)
   */
  void		setTextPosition(int x, int y);

  /**
   * \fn void setTextAttr(unsigned int style)
   * \brief Set text attributes
   * \param[in] style, a flag setting the text's attribute
   *
   * This method calls sf::Text.setStyle(style)
   */
  void		setTextAttr(unsigned int style);

  /**
   * \fn void setHidden(bool state)
   * \brief Set widget's hide attribute
   * \param[in] the hide state
   *
   * This method sets _hide to state
   */
  void		setHidden(bool state);

  /**
   * \fn void setFunction(const std::string &key, const std::function
   <int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
   &func);
   * \brief Assign a function to the widget
   * \param[in] The function to assign
   *
   */
  void		setFunction(const std::string &key, const std::function
			    <int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
			    &func);

  /**   * \fn void resize(const sf::Vector2f &size)
   * \brief Resize the widget
   * \param[in] A vector specifying the resolution to adapt to
   *
   * This method will resize all the widget's attributes
   */
  void		resize(const sf::Vector2f &size);

  /**
   * \fn void trigger(const t_event &event)
   * \brief Function called when the widget is trigged by an observer
   * \param[in] The event
   *
   * This method permit the widget to auto update its attributes when receiving externals events
   */

  void		trigger(const t_event &event);

public:		// public so the lambda can call it

  /**
   * \fn bool isOver(const sf::RenderWindow &ref)
   * \brief Check if the mouse is over the widget
   * \param[in] the Referred window
   * \return A boolean indicating if the mouse if over
   *
   */
  bool		isOver(const sf::RenderWindow &ref) const;

  /**
   * \fn bool isClicked(const sf::Event &event, sf::Mouse::Button button)
   * \brief Check if the widget is clicked
   * \param[in] Window's events and the mouse's button to check
   * \return A boolean indicating if the mouse's button was clicked
   *
   * Combining this function with the isOver function let you know if the widget is clicked
   */
  bool		isClicked(const sf::Event &event, sf::Mouse::Button button) const;

  void		eOver(unsigned int spritePos);

protected:
  bool				_hide;
  const std::string		_id;
  sf::FloatRect			_zone;
  int				_spritePos;
  std::vector<sf::Sprite>	_sprites;
  sf::Text			_text;
  std::map<std::string, std::function
	   <int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>>
    _updates;
};

inline int	operator&(wEvent a, wEvent b)
{
  return (static_cast<int>(a) & static_cast<int>(b));
};

inline wEvent	operator|(wEvent a, wEvent b)
{
  return (static_cast<wEvent>(static_cast<int>(a) | static_cast<int>(b)));
}

#endif /* _AWIDGET_H_ */
