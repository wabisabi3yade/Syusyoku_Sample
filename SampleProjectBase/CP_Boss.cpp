#include "pch.h"
#include "CP_Boss.h"

CP_Boss::CP_Boss()
	: pAnimation(nullptr)
{
}

void CP_Boss::Init()
{
	pActionController = std::unique_ptr<BossActionController>();
}
