#ifndef _DEMANGLE_HPP_
# define _DEMANGLE_HPP_

# include <string>
# include <typeinfo>
# include <stdexcept>

# include "Unused.hpp"
# include "Config.h"

# define GCC 1
# define MSVC 2
# define NONE 3

# if DEMANGLE_ABI == GCC
#  include <cxxabi.h>
# elif DEMANGLE_ABI == MSVC
#  include <Windows.h>
#  include <DbgHelp.h>
# endif

template<typename T>
std::string typeName(UNUSED T a)
{
  std::string res("");

# if DEMANGLE_ABI == GCC
  int status;
  char* demangled;

  demangled = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
  if (!demangled || status != 0)
    {
      if (status == -1)
        throw std::runtime_error("typeName: ""A memory allocation failure occurred.");
      else if (status == -2)
        throw std::runtime_error("typeName: ""mangled_name is not a valid name under the C++ ABI mangling rules.");
      else if (status == -3)
        throw std::runtime_error("typeName: ""One of the arguments is invalid.");
      else
        throw std::runtime_error("typeName: ""Unknown error.");
    }
  else
    res = demangled;
  free(demangled);
# elif DEMANGLE_ABI == MSVC
  const char* decorated_name = typeid(instance).name();
  char undecorated_name[1024];
  if (!UnDecorateSymbolName(decorated_name, undecorated_name, sizeof(undecorated_name) / sizeof(*undecorated_name), UNDNAME_COMPLETE))
    throw std::runtime_error(std::string("typeName: ") + GetLastError());
  res = undecorated_name;
# else
  throw std::runtime_error("typeName: ""No demangling ABI.");
# endif
  return res;
}

#endif /* !_DEMANGLE_HPP_ included */
