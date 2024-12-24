#include "pch.h"
#include "PlayerGuardState.h"
#include "CP_Player.h"

namespace HashiTaku
{
	constexpr auto GUARDEND_ANIM("Guard_End");
	constexpr auto GUARDKNOCK_ANIM("Guard_Knock");
	constexpr auto PARRYTRIGGER_NAME("parryTrigger");

	PlayerGuardState::PlayerGuardState() :
		canParry(false), sustainParryFrame(4), parryElapsedFrame(0), parryAddGuardGage(25.0f),
		canParryForwardAngle(180.0f)
	{
	}

	bool PlayerGuardState::GetCanParry(const DirectX::SimpleMath::Vector3& _enemyPos)
	{
		if (!canParry) return false;

		// 攻撃を正面で受けれているか確認(敵との座標)
		// プレイヤーの正面に敵がいるか確認
		Transform& transform = GetMyTransform();
		DXSimp::Vector3 toEnemyVec = _enemyPos - transform.GetPosition();
		toEnemyVec.y = 0.0f; toEnemyVec.Normalize();
		float dot = toEnemyVec.Dot(transform.Forward());

		if (dot < cos(canParryForwardAngle * Mathf::degToRad * 0.5f)) return false;

		return true;
	}

	void PlayerGuardState::OnParry()
	{
		// パリィトリガーをtrue
		GetAnimation()->SetTrigger(PARRYTRIGGER_NAME);

		// アニメーション
		GetAnimation()->SetBool(GUARD_PARAMNAME, false);

		// キャンセル全てできるようにする
		GetAnimation()->SetBool(CANCEL_PARAMNAME, true);
		GetAnimation()->SetBool(CANATK_PARAMNAME, false);

		// エフェクトを出す
		CreateParryVfx();

		// 前入力されていたら
		if (IsInputVector(InputVector::Forward))
			ReleaseAttack();	// 攻撃に派生
		else
			GuardParry();
	}

	json PlayerGuardState::Save()
	{
		auto data = PlayerGroundState::Save();
		data["canParryTime"] = sustainParryFrame;
		data["parryAngle"] = canParryForwardAngle;
		SaveJsonVector3("vfxOffset", createVfxOffset, data);
		data["vfxInfo"] = parryEffectInfo.Save();
		return data;
	}

	void PlayerGuardState::Load(const json& _data)
	{
		PlayerGroundState::Load(_data);
		LoadJsonUnsigned("canParryTime", sustainParryFrame, _data);
		LoadJsonFloat("parryAngle", canParryForwardAngle, _data);
		LoadJsonVector3("vfxOffset", createVfxOffset, _data);
		json vfxData;
		if (LoadJsonData("vfxInfo", vfxData, _data))
		{
			parryEffectInfo.Load(vfxData);
		}
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

		CP_RigidBody& rb = GetRB();
		DXSimp::Vector3 velocity;
		velocity.y = rb.GetVelocity().y;
		GetRB().SetVelocity(velocity);
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
		// 攻撃に変更
		ChangeState(PlayerState::SpecialAtkGuard);
	}

	void PlayerGuardState::GuardParry()
	{
	}

	void PlayerGuardState::CreateParryVfx()
	{
		Transform& transform = GetMyTransform();

		// パリィエフェクトを出す場所を求める
		DXSimp::Vector3 createVfxPos = transform.GetPosition();
		createVfxPos += transform.Right() * createVfxOffset.x;
		createVfxPos += transform.Up() * createVfxOffset.y;
		createVfxPos += transform.Forward() * createVfxOffset.z;

		// 再生
		DX11EffecseerManager::GetInstance()->Play(parryEffectInfo,
			createVfxPos,
			transform.GetEularAngles());
	}

	void PlayerGuardState::ImGuiDebug()
	{
		PlayerGroundState::ImGuiDebug();

		// パリィフレーム
		int imInt = static_cast<int>(sustainParryFrame);
		ImGui::DragInt("canParry", &imInt, 1, 0, 100);
		sustainParryFrame = static_cast<u_int>(imInt);

		ImGui::DragFloat("parryAngle", &canParryForwardAngle, 0.1f, 0.0f, 360.0f);

		// エフェクト
		ImGui::Text("Parry Vfx");
		ImGui::DragFloat3("offset", &createVfxOffset.x, 0.01f);
		parryEffectInfo.ImGuiCall();
	}
}