#include "pch.h"
#include "PlayerSlashHigh.h"

namespace HashiTaku
{
	PlayerGroundSlashHigh::PlayerGroundSlashHigh() :
		maxJumpMovement(5.0f),
		jumpPower(20.0f),
		judgeTimeAnimRatio(0.0f),
		jumpTimeAnimRatio(0.0f),
		prevAnimRatio(0.0f),
		prevJumpMovement(0.0f),
		isAlreadyJudge(false),
		isWillJump(false)
	{
	}

	json PlayerGroundSlashHigh::Save()
	{
		json data = PlayerAttackState::Save();

		data["jumpPower"] = jumpPower;
		data["judgeTimeAnimRatio"] = judgeTimeAnimRatio;
		data["jumpAnimRatio"] = jumpTimeAnimRatio;

		return data;
	}

	void PlayerGroundSlashHigh::Load(const json& _data)
	{
		PlayerAttackState::Load(_data);

		LoadJsonFloat("jumpPower", jumpPower, _data);
		LoadJsonFloat("judgeTimeAnimRatio", judgeTimeAnimRatio, _data);
		LoadJsonFloat("jumpAnimRatio", jumpTimeAnimRatio, _data);
	}

	void PlayerGroundSlashHigh::OnStartBehavior()
	{
		PlayerAttackState::OnStartBehavior();

		// パラメータ初期化
		isWillJump = false;
		isAlreadyJudge = false;
		prevAnimRatio = 0.0f;
		prevJumpMovement = 0.0f;
	}

	void PlayerGroundSlashHigh::UpdateBehavior()
	{
		PlayerAttackState::UpdateBehavior();

		// ジャンプ切り替えチェック
		CheckJumpUpdate();

		// ジャンプ開始するか更新処理
		CheckBeginJumpUpdate();
	}

	void PlayerGroundSlashHigh::CheckBeginJumpUpdate()
	{
		// ジャンプしないなら
		if (!isWillJump) return;

		// 現在のアニメーションの割合取得
		float curPlayAnimRatio = GetAnimation()->GetCurrentPlayRatio();

		// アニメーションの割合を超えたタイミングでないなら
		if (prevAnimRatio < jumpTimeAnimRatio &&
			curPlayAnimRatio >= jumpTimeAnimRatio)
		{
			OnJump();
		}

		// 次フレームの為の更新
		prevAnimRatio = curPlayAnimRatio;
	}

	void PlayerGroundSlashHigh::CheckJumpUpdate()
	{
		// 判断済みなら処理しない
		if (isAlreadyJudge) return;

		// 現在のアニメーションの割合取得
		float curPlayAnimRatio = GetAnimation()->GetCurrentPlayRatio();

		// 判断できるか確認
		if (CanJudge(curPlayAnimRatio))
		{
			// ジャンプ判断処理
			OnJudgeTransJump();
		}

		// 次フレームの為の更新
		prevAnimRatio = curPlayAnimRatio;
	}

	bool PlayerGroundSlashHigh::CanJudge(float _curAnimRatio)
	{
		// アニメーションの割合を超えたタイミングでないなら
		if (prevAnimRatio >= judgeTimeAnimRatio ||
			_curAnimRatio < judgeTimeAnimRatio) return false;

		return true;
	}

	void PlayerGroundSlashHigh::OnJump()
	{
		AddImpulse(Vec3::Up * jumpPower);
	}

	void PlayerGroundSlashHigh::OnJudgeTransJump()
	{
		// 攻撃ボタンを押してるなら
		/*if (pPlayerInput->GetButton(GameInput::ButtonType::Player_Attack))*/
		isWillJump = true;

		isAlreadyJudge = true;
	}

	void PlayerGroundSlashHigh::ImGuiDebug()
	{
		PlayerAttackState::ImGuiDebug();

		ImGui::DragFloat("JumpPower", &jumpPower, 0.1f, 0.0f, 1000.0f);
		ImGui::DragFloat("JudgeAnimRatio", &judgeTimeAnimRatio, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("JumpAnimRatio", &jumpTimeAnimRatio, 0.01f, 0.0f, 1.0f);
		jumpMovementCurve.ImGuiCall();
	}
}

