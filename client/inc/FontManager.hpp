#ifndef _FONTMANAGER_H_
#define _FONTMANAGER_H_

#include <SFML/Graphics.hpp>
#include "RessourceManager.hpp"
#include "Singleton.hpp"

#define FontPath "../client/assets/"

template <typename T = sf::Font>
class FontManager : public RessourceManager<T>, public Singleton< FontManager<T> >
{
public:
  virtual ~FontManager() = default;
  std::shared_ptr<T>	load(const std::string &path,
			     const std::string &name)
  {
    std::string		fullPath(path + name);
    std::shared_ptr<T>	font(new T);

    if (!font->loadFromFile(fullPath))
      throw (std::runtime_error("Can't load font " + fullPath));
    this->addObject(name, font);
    return font;
  }

  void			free(const std::string &name)
  {
    this->removeObject(name);
  }
};

#endif /* _FONTMANAGER_H_ */
