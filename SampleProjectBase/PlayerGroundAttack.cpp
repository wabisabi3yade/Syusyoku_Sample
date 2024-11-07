#include "pch.h"
#include "PlayerGroundAttack.h"

constexpr auto IDLE_ANIMNODE_NAME("Idle");	// 待機状態のアニメーションノード名
constexpr auto TARGETIDLE_ANIMNODE_NAME("TargetIdle");	// ターゲット待機状態のアニメーションノード名

void PlayerGroundAttack::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	// アニメーションの遷移先が待機状態なら待機に戻す
	if (_toAnimNodeName == IDLE_ANIMNODE_NAME || _toAnimNodeName == TARGETIDLE_ANIMNODE_NAME)
		ChangeState(PlayerState::Idle);
}
