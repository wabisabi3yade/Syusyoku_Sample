#include "pch.h"
#include "BossDamageState.h"
#include "BossActionController.h"
#include "CP_Boss.h"
#include "CP_Player.h"

namespace DXSimp = DirectX::SimpleMath;

constexpr auto DAMAGE_ANIM_NAME("KnockSmall");

BossDamageState::BossDamageState()
{
}

void BossDamageState::OnStartBehavior()
{
	// ダメージトリガーをONにする
	pActionController->SetAnimationTrigger(DAMAGETRIGGER_ANIMPARAM_NAME);

	// プレイヤーに向ける
	LookPlayer();
}

void BossDamageState::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	if (_toAnimNodeName == IDLE_ANIM_NAME || _fromAnimNodeName == DAMAGE_ANIM_NAME)
		ChangeState(BossState::Break_Idle);
}

void BossDamageState::LookPlayer()
{
	// 方向ベクトルを求める(yは無視)
	Transform& myTransform = pActionController->GetCharacter().GetTransform();
	DXSimp::Vector3 playerPos = pActionController->GetPlayer().GetTransform().GetPosition();
	DXSimp::Vector3 bossPos = myTransform.GetPosition();
	DXSimp::Vector3 targetVec = playerPos - bossPos;
	targetVec.y = 0.0f;
	targetVec.Normalize();

	// 回転する
	myTransform.SetRotation(Quat::RotateToVector(targetVec));
}
