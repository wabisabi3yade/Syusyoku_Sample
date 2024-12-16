#include "pch.h"
#include "CP_GroundCheck.h"
#include "GameObject.h"

namespace HashiTaku
{
	CP_GroundCheck::CP_GroundCheck() : pNotifyObject(nullptr)
	{
	}

	void CP_GroundCheck::SetGroundNotifyer(IGroundNotifyer& _notifyChecker)
	{
		pNotifyObject = &_notifyChecker;
	}

	void CP_GroundCheck::RemoveGroundNoifyer(IGroundNotifyer& _removeChecker)
	{
		if (pNotifyObject == &_removeChecker)
			pNotifyObject = nullptr;
	}

	void CP_GroundCheck::OnCollisionEnter(const CollisionInfo& _otherColInfo)
	{
		if (!pNotifyObject) return;
		// 地面以外なら処理しない
		if (_otherColInfo.pRigidBodyCp->GetGameObject().GetTag() != Tag::Ground) return;

		pNotifyObject->OnGroundEnter();
	}

	void CP_GroundCheck::OnCollisionExit(const CollisionInfo& _otherColInfo)
	{
		if (!pNotifyObject) return;
		// 地面以外なら処理しない
		if (_otherColInfo.pRigidBodyCp->GetGameObject().GetTag() != Tag::Ground) return;

		pNotifyObject->OnGroundExit();
	}
}