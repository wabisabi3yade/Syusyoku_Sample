#include "pch.h"
#include "CP_Weapon.h"

CP_Weapon::CP_Weapon() : isAttackCollision(true)
{
}

void CP_Weapon::SetIsAttackCollision(bool _isAttackCollision)
{
	isAttackCollision = _isAttackCollision;
}
