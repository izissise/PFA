#ifndef _ACVAR_H_
#define _ACVAR_H_

# include <map>
# include <vector>
# include <iostream>

enum cvarType {Number, String};

/*
** restrictValue content:
** default - {min, max} or {val1, val2, valX}
*/

enum class Restriction
{
  None = 0,
  Range,
  Value
};

typedef struct	s_cvar
{
  std::vector<std::string>	restrictValue;
  std::string			value;
  cvarType			type;
  Restriction			restrictType;
  s_cvar(const std::vector<std::string> &vec,
	 const std::string &val, cvarType dtype,
	 Restriction restric = Restriction::Range) :
    restrictValue(vec), value(val), type(dtype), restrictType(restric)
  {
  }
}		t_cvar;

class ACvar
{
public:
  ACvar();
  virtual ~ACvar();

  virtual void	       	setCvar(const std::string &name, const std::string &value);

  bool	isCvar(const std::string &name) const;
  const std::vector<std::string>	&getCvarInfo(const std::string &name) const;
  const std::string	&getCvar(const std::string &name) const;
  bool			addCvar(const std::string &name, t_cvar *cvar);

protected:
  std::map<std::string, t_cvar *>		_cvars;
};

#endif /* _CVAR_H_ */
