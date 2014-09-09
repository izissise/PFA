#ifndef _APANELSCREEN_H_
#define _APANELSCREEN_H_

#include <vector>
#include "Widget.hpp"

class APanelScreen : public IObserver, public Observable
{
public:
  APanelScreen();
  virtual ~APanelScreen() = 0;

  virtual int	run(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
  virtual void	draw(sf::RenderWindow &window);
  virtual void	construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels) = 0;
  bool		isHidden() const;

  void		setHide(bool hide);

protected:
  sf::Vector2f		toPixel(const sf::Vector2f &perCent,
				const sf::Vector2f &size) const;
  sf::FloatRect		toPixel(const sf::FloatRect &percent,
				const sf::Vector2f &size) const;
  void			resizeWidgets(const sf::Vector2f &size);
  void			saveTexture(sf::Texture * const texture);
  const sf::Texture	*addSpriteForWidget(AWidget * const widget,
					    const sf::Color &color,
					    const sf::Vector2f &size);
  virtual void		trigger(const t_event &event);

protected:
  bool					_hide;
  sf::Font				_font;
  std::vector<const sf::Texture *>	_textures;
  std::vector<AWidget *>		_widgets;
};

#endif /* _APANELSCREEN_H_ */
