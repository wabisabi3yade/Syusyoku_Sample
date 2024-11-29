#include "pch.h"
#include "PlayerDamageState.h"

namespace DXSimp = DirectX::SimpleMath;

PlayerDamageState::PlayerDamageState() :
	maxKnockMoveSpeed(0.0f), prevAnimRatio(0.0f), isKnockMoving(false), isInvicible(false)
{
}

void PlayerDamageState::SetKnockVec(const DirectX::SimpleMath::Vector3& _knockVec)
{
	knockVector = _knockVec;
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

	// 敵の方向を見る
	LookEnemy();

	// のけぞりに無敵をつけるか
	SetInvicible(isInvicible);

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
	// 無敵解除
	SetInvicible(false);
}

void PlayerDamageState::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	if (_toAnimNodeName == IDLE_ANIM_NAME)	// 待機に変わったら
		ChangeState(PlayerState::Idle);
}

void PlayerDamageState::KnockMove()
{
	if (!isKnockMoving) return;	// ノック移動中じゃないなら

	float animRatio = GetAnimation()->GetCurrentPlayRatio();

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

void PlayerDamageState::LookEnemy()
{
	// 敵の方向を見る
	knockVector *= -1;
	auto rot = Quat::RotateToVector(knockVector);
	GetTransform().SetRotation(rot);
}

void PlayerDamageState::ImGuiDebug()
{
	ImGui::Checkbox("Invicible", &isInvicible);
	ImGui::DragFloat("knockDistance", &maxKnockMoveSpeed, 0.1f, 0.0f, 1000.0f);
	knockSpeedCurve.ImGuiCall();
}
