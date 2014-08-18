#ifndef _CVAR_H_
#define _CVAR_H_

# include <map>
# include <vector>
# include <string>

enum cvarType {Number, String};

/*
** restrictValue content:
** default - {min, max} or {val1, val2, valX}
*/

typedef struct	s_cvar
{
  std::vector<std::string>	restrictValue;
  std::string			value;
  cvarType			type;
  s_cvar(const std::vector<std::string> &vec,
	 const std::string &val,  cvarType dtype) :
    restrictValue(vec), value(val), type(dtype)
  {
  }
}		t_cvar;

class CvarList
{
public:
  CvarList();
  virtual ~CvarList();

  const std::string	&getCvar(const std::string &name) const;
  bool			addCvar(const std::string &name, t_cvar *cvar);

private:
  std::map<std::string, t_cvar *>		_cvars;
};

#endif /* _CVAR_H_ */
