#ifndef _AWIDGET_H_
#define _AWIDGET_H_

#include <SFML/Graphics.hpp>
#include <map>
#include <functional>
#include <list>
#include "Observer.hpp"
#include "Settings.hpp"
#include "Unused.hpp"

#define SIZEX 1600.0
#define SIZEY 900.0

enum class	wEvent
{
  None = 0,
    Toggle = 1,
    Hide = 2,
    Show = 4,
    SetSprite = 16,
    UpdateText = 32
    };

enum class	wFlag
{
  None = 0,
    Movable = 1,
    Resizable = 2
    };

typedef struct	s_event
{
  wEvent	e;
  int		idx;
  int		value;
  const std::string	&strText;

  s_event(wEvent event = wEvent::None, int add = 0, int v = 0,
	  const std::string &s = "") :
    e(event), idx(add), value(v), strText(s)
  {
  }

  // s_event(wEvent event = wEvent::None, const std::string &s) :
  //   e(event), idx(0), value(0), strText(s)
  // {
  // }
}		t_event;

typedef struct	s_sprite
{
  sf::Sprite	sprite;
  bool		draw;
  s_sprite(const sf::Sprite &s, bool d = true) :
    sprite(s), draw(d)
  {
  }
}		t_sprite;

inline int	operator&(wFlag a, wFlag b)
{
  return (static_cast<int>(a) & static_cast<int>(b));
};

inline wFlag	operator|(wFlag a, wFlag b)
{
  return (static_cast<wFlag>(static_cast<int>(a) | static_cast<int>(b)));
}

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
	  const sf::Text &text, wFlag flg = wFlag::Movable | wFlag::Resizable);
  virtual ~AWidget();

  virtual int		update(const sf::Event &event, sf::RenderWindow &ref, Settings &set) = 0;

  /**
   * \fn void draw(sf::RenderWindow &window)
   * \brief Draw the widget's sprite and text.
   * \param[in] window The render window
   *
   */
  virtual void		draw(sf::RenderWindow &window) const;

  /**
   * \fn void addSprite(sf::Sprite &sprite)
   * \brief Add a sprite to the widget container.
   * \param[in] elem The sprite to store
   *
   */
  virtual void		addSprite(t_sprite &elem);

  /**
   * \fn void addSprite(const sf::Texture &texture, const sf::IntRect &rect);
   * \brief Add a sprite to the widget container.
   * \param[in] texture The used texture
   * \param[in] rect The sprite to add
   * \param[in] draw The sprite attr
   *
   * This method is used to not create a local sf::Sprite
   */
  virtual void		addSprite(const sf::Texture &texture,
				  const sf::IntRect &rect, bool draw = true);

  /**
   * \fn void alignText(const sf::Vector2f &pos, const sf::Vector2f &size, float xPercent, float yPercent);
   * \brief Align Text from it's center
   * \param[in] pos Rect(left, top)
   * \param[in] size Rect(width, height)
   * \param[in] xPercent (% width)
   * \param[in] yPercent (% height)
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
   * \param[in] pos: Rect(left, top)
   * \param[in] size: Rect(width, height)
   * \param[in] xPercent (% width)
   * \param[in] yPercent (% height)
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
   * \param[in] x position
   * \param[in] y position
   *
   * This method calls sf::Text.setPosition(x, y)
   */
  void		setTextPosition(int x, int y);

  /**
   * \fn void setTextAttr(unsigned int style)
   * \brief Set text attributes
   * \param[in] style A flag setting the text's attribute
   *
   * This method calls sf::Text.setStyle(style)
   */
  void		setTextAttr(unsigned int style);

  /**
   * \fn void setTextContent(text)
   * \brief Set text string
   * \param[in] text The new string
   *
   * This method set the string text to _text -> sf::Text
   */
  void		setTextContent(const std::string &text);

  /**
   * \fn void getTextContent()
   * \brief Return the displayed text
   *
   * This method returns a reference to a sf::String representing the _text content
   */
  const sf::String	&getTextContent() const;

  /**
   * \fn void setColor(text)
   * \brief Change the text color
   * \param[in] color The new color
   *
   */
  virtual void		setColor(const sf::Color &color);

  /**
   * \fn void setHidden(bool state)
   * \brief Set widget's hide attribute
   * \param[in] state The hide state
   *
   * This method sets _hide to state
   */
  void		setHidden(bool state);

  /**
   * \fn void isHidden()
   * \brief Get the display attr
   * \return A boolean indicating if the widget is hidden
   *
   * This method let you know if the widget is displayed
   */
  bool		isHidden() const;

  /**
   * \fn void setFunction(const std::function
   <int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
   &func);
   * \brief Assign an update function to the widget
   * \param[in] The function to assign
   *
   */
  void		setUpdate(const std::function
			  <int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>
			  &func);

  /**
   * \fn void setTrigger(const std::function  <void (const t_event &event)> &func);
   * \brief Assign a trigger function to the widget
   * \param[in] The function to assign
   *
   * The trigger function is called when the widget receives an event
   */
  void		setTrigger(const std::function  <void (const t_event &event)> &func);

  /**
   * \fn void scale(const sf::Vector2f &size)
   * \brief Scale the widget
   * \param[in] size A vector specifying the resolution to adapt to
   *
   * This method will resize all the widget's attributes
   */
  virtual void		scale(const sf::Vector2f &size);

  /**
   * \fn void resize(const sf::Vector2f &size)
   * \brief Resize the widget
   * \param[in] size A vector specifying the x/y percent
   *
   * This method will adapt the widget's size by x/y percent
   */
  virtual void		resize(float pX, float pY);

  /**
   * \fn void setSize(float pX, float pY)
   * \brief Set the widget's size
   * \param[in] pX Widget's width
   * \param[in] pY Widget's height
   *
   * This method will set the widget's size to pX, pY
   */
  virtual void		setSize(float pX, float pY);

  /**
   * \fn void setSpriteSize(unsigned int spritePos, float pX, float pY)
   * \brief Set the widget's sprite's size
   * \param[in] spritePos The sprite's index
   * \param[in] x sprite's width
   * \param[in] y sprite's height
   *
   * This method will set the widget's sprite's size to x, y
   */
  virtual void		setSpriteSize(unsigned int spritePos, float x, float y);

  /**
   * \fn void move(float pX, float pY)
   * \brief Move the widget
   * \param[in] x displacement
   * \param[in] y displacement
   *
   * This method will add pX and pY to the widgets initial positions
   */
  virtual void		move(float pX, float pY);

  /**
   * \fn void trigger(const t_event &event)
   * \brief Function called when the widget is trigged by an observer
   * \param[in] event The event
   *
   * This method permit the widget to auto update its attributes when receiving externals events
   */
  virtual void		trigger(const t_event &event);

public:		// public so the lambda can call it

  /**
   * \fn bool isOver(const sf::RenderWindow &ref)
   * \brief Check if the mouse is over the widget
   * \param[in] the Referred window
   * \return A boolean indicating if the mouse if over
   *
   */
  virtual bool		isOver(const sf::RenderWindow &ref) const;

  /**
   * \fn bool isClicked(const sf::Event &event, sf::Mouse::Button button)
   * \brief Check if the widget is clicked
   * \param[in] Window's events and the mouse's button to check
   * \return A boolean indicating if the mouse's button was clicked
   *
   * Combining this function with the isOver function let you know if the widget is clicked
   */
  bool		isClicked(const sf::Event &event, sf::Mouse::Button button) const;

  /**
   * \fn void setSpriteAttr(unsigned int spritePos, bool draw)
   * \brief Set the sprite's attributes
   * \param[in] spritePos The sprite index
   * \param[in] draw The draw value
   *
   * This method authorize you to display a part of the widget's sprites
   */
  void		setSpriteAttr(unsigned int spritePos, bool draw);

  /**
   * \fn void getSpriteAttr(unsigned int spritePos)
   * \brief Get the sprite's attributes
   * \param[in] spritePos The sprite index
   * \return The t_sprite structure
   *
   * This method returns the information of a sprite
   */
  const t_sprite	&getSpriteAttr(unsigned int spritePos) const;

  /**
   * \fn void toggleSpriteAttr(unsigned int spritePos)
   * \brief toggle the sprite's attributes
   * \param[in] spritePos The sprite index
   *
   */
  void		toggleSpriteAttr(unsigned int spritePos);

  /**
   * \fn void getSprite(unsigned int spritePos)
   * \brief Get the sprite's structure
   * \param[in] spritePos The sprite index
   * \return The t_sprite structure
   *
   * Same as getSpriteAttr but this one can be modified
   */
  t_sprite	&getSprite(unsigned int spritePos);

  /**
   * \fn void getZone()
   * \brief Get the widget's zone
   * \return A sf::FLoatRect representing the widget's zone
   *
   */
  const sf::FloatRect	&getZone() const;

  /**
   * \fn void getFlag()
   * \brief Get the widget's flag
   * \return The widget's flag, see enum class wFlag
   *
   */
  wFlag			getFlag() const;
  virtual const sf::String	&getValue() const;

protected:
  bool				_hide;
  const std::string		_id;
  sf::FloatRect			_zone;
  std::vector<t_sprite>		_sprites;
  sf::Text			_text;
  wFlag				_flag;
  std::function
  <int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)> _update;
  std::function
  <void (const t_event &event)>	_event;
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
