#ifndef _SELECTLIST_H_
#define _SELECTLIST_H_

#include "ASelectList.hpp"

class SelectList : public ASelectList
{
public:
  SelectList(const sf::FloatRect &zone);

  void	construct(const sf::Texture &texture, Settings &set,
		  const std::vector<APanelScreen *> &panels);
};

#endif /* _SELECTLIST_H_ */
