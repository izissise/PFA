#ifndef _AUDIOMANAGER_H_
#define _AUDIOMANAGER_H_

#include <SFML/Audio.hpp>
#include "RessourceManager.hpp"
#include "Singleton.hpp"

template <typename T = sf::SoundBuffer>
class AudioManager : public RessourceManager<T>, public Singleton< AudioManager<T> >
{
public:
  virtual ~AudioManager() = default;
  std::shared_ptr<T>	load(const std::string &path,
			     const std::string &name)
  {
    std::string		fullPath(path + name);
    std::shared_ptr<T>	soundBuf(new T);

    if (!soundBuf->loadFromFile(fullPath))
      throw (std::runtime_error("Can't load Audio file " + fullPath));
    this->addObject(name, soundBuf);
    return soundBuf;
  }

  void			free(const std::string &name)
  {
    this->removeObject(name);
  }
};

#endif /* _AUDIOMANAGER_H_ */
