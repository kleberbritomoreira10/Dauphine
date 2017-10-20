#include "StateBoss.h"

StateBoss::StateBoss( Boss* const BOSS ) : boss(BOSS)
{

}

StateBoss::~StateBoss()
{
  this->boss = nullptr;
}
