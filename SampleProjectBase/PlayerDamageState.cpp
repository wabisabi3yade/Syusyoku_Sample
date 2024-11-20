#include "pch.h"
#include "PlayerDamageState.h"

namespace DXSimp = DirectX::SimpleMath;

PlayerDamageState::PlayerDamageState() : 
	maxKnockMoveSpeed(0.0f), prevAnimRatio(0.0f), isKnockMoving(false)
{
}

nlohmann::json PlayerDamageState::Save()
{
	auto data = PlayerActState_Base::Save();

	data["knockDis"] = maxKnockMoveSpeed;
	data["knockCurve"] = knockSpeedCurve.Save();

	return data;
}

void PlayerDamageState::Load(const nlohmann::json& _data)
{
	PlayerActState_Base::Load(_data);

	HashiTaku::LoadJsonFloat("knockDis", maxKnockMoveSpeed, _data);
	if (HashiTaku::IsJsonContains(_data, "knockCurve"))
	{
		knockSpeedCurve.Load(_data["knockCurve"]);
	}
}

void PlayerDamageState::OnStartBehavior()
{
	// アニメーションに通知
	GetAnimation()->SetTrigger(DAMAGETRIGGER_PARAMNAME);

	// リセット
	prevAnimRatio = 0.0f;
	isKnockMoving = true;
}

void PlayerDamageState::UpdateBehavior()
{
	KnockMove();
}

void PlayerDamageState::OnEndBehavior()
{
}

void PlayerDamageState::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	if(_toAnimNodeName == IDLE_ANIM_NAME)	// 待機に変わったら
		ChangeState(PlayerState::Idle);
}

void PlayerDamageState::KnockMove()
{
	if (!isKnockMoving) return;	// ノック移動中じゃないなら

	float animRatio = GetAnimation()->GetCurrentAnimationRatio();

	// 前回の割合より前に来たなら
	if (prevAnimRatio > animRatio)
	{
		isKnockMoving = false;	// 移動を終える
		GetRB().SetVelocity(DXSimp::Vector3::Zero);
		return;
	}

	// 現在の吹っ飛んだ距離を取得
	float curKnockSpeed = knockSpeedCurve.GetValue(animRatio) * maxKnockMoveSpeed;

	// 移動速度を求める(後ろに移動)
	DXSimp::Vector3 moveSpeed =
		-GetTransform().Forward() * curKnockSpeed;
	GetRB().SetVelocity(moveSpeed);

	prevAnimRatio = animRatio;
}

void PlayerDamageState::ImGuiDebug()
{
	ImGui::DragFloat("knockDistance", &maxKnockMoveSpeed, 0.1f, 0.0f, 1000.0f);
	knockSpeedCurve.ImGuiCall();
}
