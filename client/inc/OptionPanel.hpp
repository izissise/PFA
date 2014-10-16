#ifndef _OPTIONPANEL_H_
#define _OPTIONPANEL_H_

#include "APanelScreen.hpp"

class OptionPanel : public APanelScreen
{
public:
  OptionPanel(const sf::FloatRect &zone);
  virtual ~OptionPanel();

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);
  void		trigger(const t_event &event);
};

#endif /* _OPTIONPANEL_H_ */
