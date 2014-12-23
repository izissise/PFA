#ifndef _APANELSCREEN_H_
#define _APANELSCREEN_H_

#include <vector>
#include "AWidget.hpp"

class APanelScreen : public IObserver, public Observable
{
public:
  enum	State
    {
      None = 0,
      Leader = 1
    };

public:
  APanelScreen(const sf::FloatRect &zone);
  virtual ~APanelScreen() = 0;

  /**
   * \fn int run(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
   * \brief Update the state of the panel
   * \param[in] event The event
   * \param[in] ref The reffered window
   * \param[in] set The game's settings
   * \return Return a code indicating if the event has been treated
   *
   * This function will call every widget's update method
   */
  virtual int	update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);

  /**
   * \fn void draw(sf::RenderWindow &window)
   * \brief Draw the Panel
   * \param[in] window The render window
   * \param[in] toWin boolean indicating where to draw
   *
   * The panel's draw method will call every widget's draw method
   */
  virtual void	draw(sf::RenderWindow &window, bool toWin);

  bool	checkPanelBounds(AWidget * const widget) const;
  /**
   * \fn void print(sf::RenderTexture &rt)
   * \brief Draw the Panel's content
   * \param[in] rt The main renderTexture
   *
   * print cut a sprite out of the panel texture to apply it on the main texture
   */
  void	print(sf::RenderTexture &rt) const;

  /**
   * \fn void construct(const sf::Texture &texture, Settings &set, const std::vector<APanelScreen *> &panels) = 0;
   * \brief Implements the widgets in the panel
   * \param[in] texture The main texture,
   * \param[in] set The game's settings
   * \param[in] panels A list of panels
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
   * \param[in] hide The hide state
   *
   */
  void		setHide(bool hide);
  void		setState(APanelScreen::State state);
  void		move(const sf::Vector2f &displacement);
  void		setPosition(const sf::Vector2f &position);
  APanelScreen::State	getState() const;
  void		setTrigger(const std::function<void (const t_event &event)> &func);
  void		addPanel(APanelScreen * const panel);
  void	       	addPanel(const std::initializer_list<APanelScreen * const> &panels);
  virtual void	addWidget(AWidget * const widget);
  virtual void	addWidget(const std::initializer_list<AWidget * const> &widgets);
  void		addFont(const std::string &fontName, const std::string &fontPath);

  /**
   * \fn const sf::Texture *addSpriteForWidget(AWidget * const widget, const sf::Color &color, const sf::Vector2f &size)
   * \brief Add a sprite to a widget
   * \param[in] widget The widget
   * \param[in] color Sprite's color
   * \param[in] size Sprite's size
   * \param[in] draw Specify if by default the sprite is displayed
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
   * \fn const std::vector<AWidget *> &getWidgets() const;
   * \brief Getter on the panel's widgets
   * \return Return a const reference to a vector containing the panel's widgets
   *
   */
  const std::vector<AWidget *>	&getWidgets() const;

  /**
   * \fn const std::vector<APanelScreen *> &getSubPanels() const;
   * \brief Getter on the panel's subPanels
   * \return Return a const reference to the panel's subPanels
   *
   */
  const std::vector<APanelScreen *> &getSubPanels() const;

  /**
   * \fn std::vector<APanelScreen *> &getSubPanels();
   * \brief Getter on the panel's subPanels
   * \return Return a const reference to the panel's subPanels
   *
   */
  std::vector<APanelScreen *> &getSubPanels();

  /**
   * \fn const sf::FloatRect &getZone() const;
   * \brief Getter on the panel's zone
   * \return Return a const reference to the panel's zone (it's size and position)
   *
   */
  const sf::FloatRect		&getZone() const;

protected:
  /**
   * \fn void toPixel(const sf::Vector2f &perCent, const sf::Vector2f &size)
   * \brief Resize 2dVector by x/y percent
   * \param[in] perCent The resizing percentage
   * \param[in] size The vector to resize
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
   * \param[in] percent The resizing percentage
   * \param[in] rect The rectangle to resize
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
   * \param[in] size A vector containing the window's size
   *
   * This calls the widget's resize method
   */
  void			resizeWidgets(const sf::Vector2f &size);

  /**
   * \fn void saveTexture(sf::Texture * const texture)
   * \brief Save a texture used by the panel
   * \param[in] texture The texture to save
   *
   */
  void			saveTexture(sf::Texture * const texture);

  /**
   * \fn void trigger(const t_event &event)
   * \brief Trigger the panel from an event
   * \param[in] event A structure containing the event's informations
   *
   * This allow a widget to update a status of a panel.
   * Example: click on a button -> display the panel
   *
   */
  virtual void		trigger(const t_event &event);

private:
  /**
   * \fn const sf::RenderTexture &getRT() const
   * \brief Getter on the panel's renderTexture
   * \return Return a const reference to the panel's renderTexture
   *
   */
  const sf::RenderTexture	&getRT() const;
  void	moveWidgets(APanelScreen * const pan,
		    const sf::Vector2f &displacement);

protected:
  bool					_hide;
  sf::FloatRect				_zone;
  State					_state;
  sf::RenderTexture			_rt;
  std::map<std::string, sf::Font>	_font;
  std::function<void (const t_event &event)>	_trigger;
  std::vector<const sf::Texture *>	_textures;
  std::vector<APanelScreen *>		_panels;
  std::vector<AWidget *>		_widgets;
};

#endif /* _APANELSCREEN_H_ */
