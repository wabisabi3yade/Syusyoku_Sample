#include "pch.h"
#include "PlayerGuardState.h"
#include "CP_Player.h"
#include "CP_SoundManager.h"

namespace HashiTaku
{
	constexpr auto GUARDEND_ANIM("Guard_End");
	constexpr auto GUARDKNOCK_ANIM("Guard_Knock");
	constexpr auto PARRYTRIGGER_NAME("parryTrigger");

	PlayerGuardState::PlayerGuardState() :
		canParry(false), 
		sustainParryFrame(4),
		parryElapsedFrame(0),
		parryAddGuardGage(25.0f),
		canParryForwardAngle(180.0f),
		addStylishPointOnParry(200.0f)
	{
	}

	bool PlayerGuardState::GetCanParry(const DXSimp::Vector3& _enemyPos)
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
	/*	GetAnimation()->SetBool(CANCEL_PARAMNAME, true);
		GetAnimation()->SetBool(CANATK_PARAMNAME, false);*/

		// エフェクトを出す
		CreateParryVfx();

		// 効果音を鳴らす
		PlayParrySE();

		// スタイリッシュポイントを加算
		GetPlayer().AddStylishPoint(addStylishPointOnParry);

		// 前入力されていたら
		if (IsInputVector(InputVector::Forward))
			ReleaseAttack();	// 攻撃に派生
	}

	json PlayerGuardState::Save()
	{
		auto data = PlayerGroundState::Save();
		data["canParryTime"] = sustainParryFrame;
		data["parryAngle"] = canParryForwardAngle;
		SaveJsonVector3("vfxOffset", createVfxOffset, data);
		data["vfxInfo"] = parryEffectInfo.Save();
		data["addStylishPointParry"] = addStylishPointOnParry;
		for (auto& itr : parrySoundParameters)
		{
			data["parrySEs"].push_back(itr.Save());
		}

		return data;
	}

	void PlayerGuardState::Load(const json& _data)
	{
		PlayerGroundState::Load(_data);
		LoadJsonUnsigned("canParryTime", sustainParryFrame, _data);
		LoadJsonFloat("parryAngle", canParryForwardAngle, _data);
		LoadJsonVector3("vfxOffset", createVfxOffset, _data);
		LoadJsonFloat("addStylishPointParry", addStylishPointOnParry, _data);
		json loadData;
		if (LoadJsonData("vfxInfo", loadData, _data))
		{
			parryEffectInfo.Load(loadData);
		}

		if (LoadJsonDataArray("parrySEs", loadData, _data))
		{
			for (auto& soundData : loadData)
			{
				PlaySoundParameter loadSound;
				loadSound.Load(soundData);
				parrySoundParameters.push_back(loadSound);
			}
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

	void PlayerGuardState::PlayParrySE()
	{
		// シーン内にサウンドマネジャーがあるか確認
		CP_SoundManager* pSoundManager = CP_SoundManager::GetInstance();
		if (!pSoundManager) return;

		// ランダムで再生する音を変える
		int soundCnt = static_cast<int>(parrySoundParameters.size());
		if (soundCnt == 0) return;

		int randId = Random::RangeInt<int>(0, soundCnt - 1);
		auto soundItr = parrySoundParameters.begin();
		soundItr = std::next(soundItr, randId);

		// プレイヤーからSEを再生
		pSoundManager->PlaySE(*soundItr, GetMyTransform().GetPosition());
	}

	void PlayerGuardState::ImGuiDebug()
	{
		PlayerGroundState::ImGuiDebug();

		// パリィ
		int imInt = static_cast<int>(sustainParryFrame);
		ImGui::DragInt("CanParry", &imInt, 1, 0, 100);
		sustainParryFrame = static_cast<u_int>(imInt);
		ImGui::DragFloat("ParryAngle", &canParryForwardAngle, 0.1f, 0.0f, 360.0f);
		ImGui::DragFloat("AddStylishPoint", &addStylishPointOnParry, 0.1f, 0.0f, 10000.0f);

		// エフェクト
		ImGuiMethod::LineSpaceSmall();
		ImGui::Text("Parry Vfx");
		ImGui::DragFloat3("Offset", &createVfxOffset.x, 0.01f);
		parryEffectInfo.ImGuiCall();

		// サウンド
		ImGuiMethod::LineSpaceSmall();
		ImGui::Text("Parry SE");
		u_int loop = 1;
		for (auto itr = parrySoundParameters.begin(); itr != parrySoundParameters.end();)
		{
			bool isDelete = false;
			if (ImGuiMethod::TreeNode(std::to_string(loop)))
			{
				if (ImGui::Button("X"))
					isDelete = true;
				
				itr->ImGuiCall();

				ImGui::TreePop();
			}
			loop++;

			if (isDelete)
				itr = parrySoundParameters.erase(itr);
			else
				itr++;
		}
		if (ImGui::Button("+"))
		{
			PlaySoundParameter add;
			parrySoundParameters.push_back(add);
		}
	}
}