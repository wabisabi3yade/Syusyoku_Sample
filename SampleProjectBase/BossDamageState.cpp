#include "pch.h"
#include "BossDamageState.h"
#include "BossActionController.h"
#include "CP_Boss.h"
#include "CP_Player.h"

namespace DXSimp = DirectX::SimpleMath;

BossDamageState::BossDamageState()
{
}

void BossDamageState::OnStartBehavior()
{
	BossActState_Base::OnStartBehavior();

	HASHI_DEBUG_LOG("a");

	// �_���[�W�g���K�[��ON�ɂ���
	pActionController->SetAnimationTrigger(DAMAGETRIGGER_ANIMPARAM_NAME);

	// �v���C���[�Ɍ�����
	LookPlayer();
}

void BossDamageState::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	if (_toAnimNodeName == IDLE_ANIM_NAME)
		ChangeState(BossState::Idle);
}

void BossDamageState::LookPlayer()
{
	// �����x�N�g�������߂�(y�͖���)
	Transform& myTransform = pActionController->GetCharacter().GetTransform();
	DXSimp::Vector3 playerPos = pActionController->GetPlayer().GetTransform().GetPosition();
	DXSimp::Vector3 bossPos = myTransform.GetPosition();
	DXSimp::Vector3 targetVec = playerPos - bossPos;
	targetVec.y = 0.0f;
	targetVec.Normalize();

	// ��]����
	myTransform.SetRotation(Quat::RotateToVector(targetVec));
}
