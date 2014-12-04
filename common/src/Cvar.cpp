#include <algorithm>
#include "ACvar.hpp"
#include "Exception.hpp"

ACvar::ACvar()
{
}

ACvar::~ACvar()
{
}

bool	ACvar::isCvar(const std::string &name) const
{
  auto it = _cvars.find(name);

  return it != _cvars.end();
}

const std::array<std::string, 3>	&ACvar::getCvarInfo(const std::string &name) const
{
  auto it = _cvars.find(name);

  if (it == _cvars.end())
    throw(Exception("Cvar [" + name + "] doesn't exist"));
  return it->second->restrictValue;
}

const std::string	&ACvar::getCvar(const std::string &name) const
{
  auto it = _cvars.find(name);

  if (it == _cvars.end())
    throw(Exception("Cvar [" + name + "] doesn't exist"));
  return it->second->value;
}

bool	ACvar::addCvar(const std::string &name, t_cvar *cvar)
{
  auto ret = _cvars.insert(std::pair<std::string, t_cvar *>(name, cvar));

  return ret.second;
}

void	ACvar::setCvar(const std::string &name, const std::string &value)
{
  auto it = _cvars.find(name);
  t_cvar	*cvar;
  double	numValue;

  if (it == _cvars.end())
    throw(Exception("Cvar [" + name + "] doesn't exist"));
  cvar = it->second;
  if ((cvar->type == String && cvar->restrictValue.size() > 1) ||
      (cvar->type == Number && cvar->restrictValue.size() > 3))
    {
      if (std::find(cvar->restrictValue.begin(), cvar->restrictValue.end(), value) ==
          cvar->restrictValue.end())
        throw(Exception("Value for " + name + " isn't correct, default: " +
                        * (cvar->restrictValue.begin())));
    }
  else if (cvar->type == Number)
    {
      try
        {
          numValue = std::stod(value);
        }
      catch (const std::invalid_argument &ia)
        {
          throw (Exception(name + " cvar's value must be a number"));
        }
      if (cvar->restrictValue.size() > 1) // Means the cvar has value limits
        {
          if (numValue < std::stod(*(cvar->restrictValue.begin() + 1)))
            throw (Exception("Value for " + name + ": " + value + ", minimum: " +
                             * (cvar->restrictValue.begin() + 1)));
          if (cvar->restrictValue.size() > 2 &&
              numValue > std::stod(*(cvar->restrictValue.begin() + 2)))
            throw (Exception("Value for " + name + ": " + value + ", maximum: " +
                             * (cvar->restrictValue.begin() + 2)));
        }
    }
  cvar->value = value;
}
