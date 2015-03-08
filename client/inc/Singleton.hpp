#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <mutex>
#include <functional>
#include <memory>
#include <utility>

template<class SingleClass>
class Singleton
{
public:
  Singleton() = default;
  virtual ~Singleton() = default;
  Singleton(const Singleton&) = delete;
  Singleton &operator=(const Singleton&) = delete;

  template<typename IMPL = SingleClass>
  static SingleClass	&instance()
  {
    std::call_once(get_once_flag(), []()
		   {
		     _instance.reset(new IMPL());
		   });
    return *(_instance.get());
  };

private:
  static std::unique_ptr<SingleClass> _instance;
  static std::once_flag& get_once_flag()
  {
    static std::once_flag once;
    return once;
  };
  static std::once_flag	_flag;
};

template<class T>
std::unique_ptr<T> Singleton<T>::_instance = nullptr;

#endif /* _SINGLETON_H_ */
