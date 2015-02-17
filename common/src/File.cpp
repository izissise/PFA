#include <stdexcept>
#include <string>
#include "File.hpp"

File::File(char separator) :
  _sep(separator)
{
}

void	File::readFile(const std::string &fileName,
		       std::vector<std::string> &content,
		       const std::ios_base::openmode &mode) const
{
  std::ifstream		file;
  std::string		line;

  file.open(fileName, mode);
  if (!file)
    throw std::invalid_argument(fileName + " : File not found");
  while (std::getline(file, line))
    content.push_back(line);
  file.close();
}

void	File::writeFile(const std::string &fileName,
			const std::vector<std::string> &content,
			const std::ios_base::openmode &mode) const
{
  std::ofstream		file;

  file.open(fileName, mode);
  if (!file)
    throw std::invalid_argument(fileName + " : Cannot open");
  for (const std::string &s : content)
    file << s << _sep;
  file.close();
}
