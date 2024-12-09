#include "pch.h"
#include "CP_GroundCheck.h"
#include "GameObject.h"

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

void CP_GroundCheck::OnCollisionEnter(const HashiTaku::CollisionInfo& _otherColInfo)
{
	if (!pNotifyObject) return;
	// �n�ʈȊO�Ȃ珈�����Ȃ�
	if (_otherColInfo.pRigidBodyCp->GetGameObject().GetTag() != HashiTaku::Tag::Ground) return;

	pNotifyObject->OnGroundEnter();
}

void CP_GroundCheck::OnCollisionExit(const HashiTaku::CollisionInfo& _otherColInfo)
{
	if (!pNotifyObject) return;
	// �n�ʈȊO�Ȃ珈�����Ȃ�
	if (_otherColInfo.pRigidBodyCp->GetGameObject().GetTag() != HashiTaku::Tag::Ground) return;

	pNotifyObject->OnGroundExit();
}
