#ifndef _FILE_H_
#define _FILE_H_

#include <fstream>
#include <vector>

class File
{
public:
  File(char separator = '\n');
  ~File() = default;

  void		readFile(const std::string &fileName,
			 std::vector<std::string> &content,
			 const std::ios_base::openmode &mode = std::ios_base::in) const;
  void		writeFile(const std::string &fileName,
			  const std::vector<std::string> &content,
			  const std::ios_base::openmode &mode = std::ios_base::out) const;
private:
  char		_sep;
};

#endif /* _FILE_H_ */
