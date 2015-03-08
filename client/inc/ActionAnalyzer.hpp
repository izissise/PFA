#ifndef _ACTIONANALYZER_H_
#define _ACTIONANALYZER_H_

#include <vector>
#include "Settings.hpp"
#include "ISerialize.hpp"

class ActionAnalyzer : public ISerialize
{
public:
  ActionAnalyzer();
  virtual ~ActionAnalyzer();

  unsigned int	getInputChanges(Settings &set);
  std::string	serialize();

private:
  std::vector<bool>	_oldActionState;
  std::vector<char>	_diffState;
};

#endif /* _ACTIONANALYZER_H_ */
