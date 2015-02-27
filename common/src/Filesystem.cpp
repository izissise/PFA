#include "Filesystem.hpp"

bool	Filesystem::createDirectory(const std::string &path,
				    const std::string &name)
{
  fs::path	p(path + name);

  return fs::create_directory(p);
}

bool	Filesystem::exist(const std::string &path,
			  const std::string &name) const
{
  return fs::exists(path + name);
}
