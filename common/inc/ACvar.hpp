#ifndef _ACVAR_H_
#define _ACVAR_H_

# include <map>
# include <array>
# include <iostream>

enum cvarType {Number, String};

/*
** restrictValue content:
** default - {min, max} or {val1, val2, valX}
*/

typedef struct	s_cvar
{
  std::array<std::string, 3>	restrictValue;
  std::string			value;
  cvarType			type;
  s_cvar(const std::array<std::string, 3> &vec,
	 const std::string &val,  cvarType dtype) :
    restrictValue(vec), value(val), type(dtype)
  {
  }
}		t_cvar;

class ACvar
{
public:
  ACvar();
  virtual ~ACvar();

  bool	isCvar(const std::string &name) const;
  const std::array<std::string, 3>	&getCvarInfo(const std::string &name) const;
  const std::string	&getCvar(const std::string &name) const;
  void			setCvar(const std::string &name, const std::string &value);
  bool			addCvar(const std::string &name, t_cvar *cvar);

protected:
  std::map<std::string, t_cvar *>		_cvars;
};

#endif /* _CVAR_H_ */
