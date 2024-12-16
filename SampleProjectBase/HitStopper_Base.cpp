#include "pch.h"
#include "HitStopper_Base.h"
#include "CP_HitStopManager.h"

namespace HashiTaku
{
	HitStopper_Base::~HitStopper_Base()
	{
		RemoveFromHitStopManager();
	}

	void HitStopper_Base::AddToHitStopManager()
	{
		if (CP_HitStopManager* pHitStopManager = CP_HitStopManager::GetInstance())
		{
			pHitStopManager->AddHitStopper(*this);
		}
	}

	void HitStopper_Base::RemoveFromHitStopManager()
	{
		if (CP_HitStopManager* pHitStopManager = CP_HitStopManager::GetInstance())
		{
			pHitStopManager->RemoveHitStopper(*this);
		}
	}
}