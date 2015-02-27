#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

class Filesystem
{
public:
  Filesystem() = default;
  ~Filesystem() = default;

  bool	createDirectory(const std::string &path,
			const std::string &name);
  bool	exist(const std::string &path,
	      const std::string &name) const;
};

#endif /* _FILESYSTEM_H_ */
