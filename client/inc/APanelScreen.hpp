#ifndef _APANELSCREEN_H_
#define _APANELSCREEN_H_

#include <vector>
#include "AWidget.hpp"

class APanelScreen : public IObserver, public Observable
{
public:
  APanelScreen();
  virtual ~APanelScreen() = 0;

  /**
   * \fn int run(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
   * \brief Update the state of the panel
   * \param[in] The event, the reffered window and the settings
   * \return Return a code indicating if the event has been treated
   *
   * This function will call every widget's update method
   */
  virtual int	run(const sf::Event &event, sf::RenderWindow &ref, Settings &set);

  /**
   * \fn void draw(sf::RenderWindow &window)
   * \brief Draw the Panel
   * \param[in] The render window
   *
   * The panel's draw method will call every widget's draw method
   */
  virtual void	draw(sf::RenderWindow &window);

  /**
   * \fn void construct(const sf::Texture &texture, Settings &set, const std::vector<APanelScreen *> &panels) = 0;
   * \brief Implements the widgets in the panel
   * \param[in] The main texture, the settings and a reference to some panels
   *
   * Construct method creates the widgets contained in the panel
   * panels are passed as parameters so a widget can trigger it
   */
  virtual void	construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels) = 0;

  /**
   * \fn bool isHidden()
   * \brief Check if the pannel is hidden
   * \return A boolean indicating if the panel is hidden
   *
   */
  bool		isHidden() const;

  /**
   * \fn bool setHide(bool hide)
   * \brief Sets the _hide panel's attribute
   *
   */
  void		setHide(bool hide);
  void	       	addPanels(const std::vector<APanelScreen *> &panels);

protected:
  /**
   * \fn void toPixel(const sf::Vector2f &perCent, const sf::Vector2f &size)
   * \brief Resize 2dVector by x/y percent
   * \param[in] The resizing percentage, the vector to resize
   * \return Return the resized vector
   *
   * return sf::Vector2f(perCent.x * size.x, perCent.y * size.y)
   * Usefull if you want to move a point (for example when changing of resolution)
   *
   */
  sf::Vector2f		toPixel(const sf::Vector2f &perCent,
				const sf::Vector2f &size) const;

  /**
   * \fn void toPixel(const sf::FloatRect &percent, const sf::Vector2f &size)
   * \brief Resize a rect by x/y percent
   * \param[in] The resizing percentage, the rectangle to resize
   * \return Return the resized rectangle
   *
   * return sf::FloatRect(rect.left * perCent.x, rect.top * perCent.y, rect.width * perCent.x, rect.height * perCent.y);
   * Usefull if you want to move and resize a rectangle (for example when changing of resolution)
   *
   */
  sf::FloatRect		toPixel(const sf::Vector2f &percent,
				const sf::FloatRect &rect) const;

  /**
   * \fn void resizeWidgets(const sf::Vector2f &size);
   * \brief Resizes the contained widgets
   * \param[in] A vector containing the window's size
   *
   * This calls the widget's resize method
   */
  void			resizeWidgets(const sf::Vector2f &size);

  /**
   * \fn void saveTexture(sf::Texture * const texture)
   * \brief Save a texture used by the panel
   * \param[in] The texture to save
   *
   */
  void			saveTexture(sf::Texture * const texture);

  /**
   * \fn const sf::Texture *addSpriteForWidget(AWidget * const widget, const sf::Color &color, const sf::Vector2f &size)
   * \brief Add a sprite to a widget
   * \param[in] widget: The widget, color: the color of the sprite, size: the size of the sprite, draw: specify if by default the sprite is displayed
   * \return Return the texture
   *
   * This method create the needed texture, store it inside the panel.
   * Then it creates the sprite and load it to the widget
   *
   */
  const sf::Texture	*addSpriteForWidget(AWidget * const widget,
					    const sf::Color &color,
					    const sf::Vector2f &size,
					    bool display = true);

  /**
   * \fn void trigger(const t_event &event)
   * \brief Trigger the panel from an event
   * \param[in] event: A structure containing a flag and an int
   *
   * This allow a widget to update a status of a panel.
   * Example: click on a button -> display the panel
   *
   */
  virtual void		trigger(const t_event &event);

protected:
  bool					_hide;
  sf::Font				_font;
  std::vector<const sf::Texture *>	_textures;
  std::vector<APanelScreen *>		_panels;
  std::vector<AWidget *>		_widgets;
};

#endif /* _APANELSCREEN_H_ */
