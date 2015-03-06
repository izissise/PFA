#ifndef _AITEM_H_
#define _AITEM_H_

class AItem
{
public:
  AItem(int life);
  virtual ~AItem() = 0;

  bool		takeDamages(unsigned int amount);
protected:
  unsigned int	_life;
};

#endif /* _AITEM_H_ */
