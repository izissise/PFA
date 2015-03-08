#ifndef _ATOOL_H_
#define _ATOOL_H_

class ATool
{
public:
  ATool(unsigned int damages);
  virtual ~ATool() = 0;

  unsigned int	getDamages() const;

protected:
  unsigned int	_damages;
};

#endif /* _ATOOL_H_ */
