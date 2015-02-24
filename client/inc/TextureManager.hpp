#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include <SFML/Graphics.hpp>
#include "RessourceManager.hpp"
#include "Singleton.hpp"

#define TexturePath "../client/assets/"

template <typename T = sf::Texture>
class TextureManager : public RessourceManager<T>, public Singleton< TextureManager<T> >
{
public:
  virtual ~TextureManager() = default;
  std::shared_ptr<T>	load(const std::string &path,
			     const std::string &name)
  {
    std::string		fullPath(path + name);
    std::shared_ptr<T>	texture(new T);

    if (!texture->loadFromFile(fullPath))
      throw (std::runtime_error("Can't load texture " + fullPath));
    this->addObject(name, texture);
    return texture;
  }

  void			free(const std::string &name)
  {
    this->removeObject(name);
  }
};

#endif /* _TEXTUREMANAGER_H_ */
