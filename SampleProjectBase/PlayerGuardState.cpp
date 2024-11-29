#include "pch.h"
#include "PlayerGuardState.h"
#include "CP_Player.h"

constexpr auto GUARDEND_ANIM("Guard_End");
constexpr auto GUARDKNOCK_ANIM("Guard_Knock");
constexpr auto PARRYTRIGGER_NAME("parryTrigger");

PlayerGuardState::PlayerGuardState() : 
	canParry(false), sustainParryFrame(4), parryElapsedFrame(0), parryAddGuardGage(25.0f)
{
}

bool PlayerGuardState::GetCanParry() const
{
	return canParry;
}

void PlayerGuardState::OnParry()
{
	// パリィでガードゲージを増やす
	GetPlayer().AddGuardGage(parryAddGuardGage);

	// パリィトリガーをtrue
	GetAnimation()->SetTrigger(PARRYTRIGGER_NAME);

	// アニメーション
	GetAnimation()->SetBool(GUARD_PARAMNAME, false);

	// 前入力されていたら
	if (IsInputVector(InputVector::Forward))
		ReleaseAttack();	// 攻撃に派生
	else
		GuardParry();
}

nlohmann::json PlayerGuardState::Save()
{
	auto data = PlayerActState_Base::Save();
	data["canParryTime"] = sustainParryFrame;
	return data;
}

void PlayerGuardState::Load(const nlohmann::json& _data)
{
	PlayerActState_Base::Load(_data);
	HashiTaku::LoadJsonUnsigned("canParryTime", sustainParryFrame, _data);
}

void PlayerGuardState::OnStartBehavior()
{
	// パリィできる状態にする
	canParry = true;
	parryElapsedFrame = 0;

	// アニメーション
	GetAnimation()->SetBool(GUARD_PARAMNAME, true);
}

void PlayerGuardState::UpdateBehavior()
{
	// パリィできる時間の更新
	ParryTimeUpdate();
}

void PlayerGuardState::OnEndBehavior()
{
	// パリィできなくする
	canParry = false;
	
	// アニメーションにガード終了通知
	GetAnimation()->SetBool(GUARD_PARAMNAME, false);
}

void PlayerGuardState::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	// ガード終了アニメーションが終われば
	if (_fromAnimNodeName == GUARDEND_ANIM || _fromAnimNodeName == GUARDKNOCK_ANIM)
		ChangeState(PlayerState::Idle);
}

void PlayerGuardState::ParryTimeUpdate()
{
	if (!canParry) return;

	// 進める
	parryElapsedFrame++;

	// パリィできるフレームでなくなったらパリィできない状態へ
	if (parryElapsedFrame > sustainParryFrame)
	{
		canParry = false;
		GetAnimation()->SetBool(GUARD_PARAMNAME, false);
	}
}

void PlayerGuardState::ReleaseAttack()
{
	// ガードゲージを消費して
	GetPlayer().ResetGuardGage();

	// 攻撃に変更
	ChangeState(PlayerState::SpecialAtkGuard);
}

void PlayerGuardState::GuardParry()
{
}

void PlayerGuardState::ImGuiDebug()
{
	PlayerActState_Base::ImGuiDebug();

	// パリィフレーム
	int imInt = static_cast<int>(sustainParryFrame);
	ImGui::DragInt("canParry", &imInt, 1, 0, 100);
	sustainParryFrame = static_cast<u_int>(imInt);

	HASHI_DEBUG_LOG("parry");
}
