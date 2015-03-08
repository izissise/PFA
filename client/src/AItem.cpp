#include "AItem.hpp"

AItem::AItem(int life):
  _life(life)
{
}

bool	AItem::takeDamages(unsigned int amount)
{
  if (amount >= _life)
    {
      _life = 0;
      return true;
    }
  _life -= amount;
  return false;
}
