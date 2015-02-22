#ifndef MAKERESSOURCE_HPP_INCLUDED
#define MAKERESSOURCE_HPP_INCLUDED

#include <memory>
#include <functional>

//
// http://ericscottbarr.com/blog/2014/04/c-plus-plus-14-and-sdl2-managing-resources/
//
template<typename Creator, typename Destructor, typename... Arguments>
auto make_resource(Creator c, Destructor d, Arguments&&... args)
    -> std::unique_ptr<typename std::decay<decltype(*c(std::forward<Arguments>(args)...))>::type, std::function<void(typename std::decay<decltype(*c(std::forward<Arguments>(args)...))>::type*)>>
{
    auto r = c(std::forward<Arguments>(args)...);
    if (!r) { throw std::runtime_error {"Unable to create resource"}; }
    typedef typename std::decay<decltype(*r)>::type ResourceType;
    return std::unique_ptr<ResourceType, std::function<void(ResourceType*)>>(r, d);
}

#endif // MAKERESSOURCE_HPP_INCLUDED
