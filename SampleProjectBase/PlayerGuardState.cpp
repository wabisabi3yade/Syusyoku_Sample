#include "pch.h"
#include "PlayerGuardState.h"
#include "CP_Player.h"
#include "CP_SoundManager.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	constexpr auto GUARDEND_ANIM("Guard_End");
	constexpr auto GUARDKNOCK_ANIM("Guard_Knock");
	constexpr auto PARRYTRIGGER_NAME("parryTrigger");

	PlayerGuardState::PlayerGuardState() :
		pCameraMove(nullptr),
		sustainParryFrame(4),
		parryElapsedFrame(0),
		parryAddGuardGage(25.0f),
		canParryForwardAngle(180.0f),
		addStylishPointOnParry(200.0f),
		parryPadVibePower(0.4f),
		parryPadVibeTime(0.2f),
		canParry(false),
		isPerformParryFrame(false)
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

	void PlayerGuardState::SetPerfomParry()
	{
		// 次の更新処理でパリィ処理を行う
		isPerformParryFrame = true;
	}

	void PlayerGuardState::OnParry()
	{
		// パリィトリガーをtrue
		GetAnimation()->SetTrigger(PARRYTRIGGER_NAME);

		// アニメーション
		GetAnimation()->SetBool(GUARD_PARAMNAME, false);

		// エフェクトを出す
		CreateParryVfx();

		// 効果音を鳴らす
		PlayParrySE();

		// カメラをシェイク
		CameraShakeOnParry();

		// コントローラーを振動させる
		PadVibrationOnParry();

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
		data["parryCamShake"] = parryCamShakeParam.Save();
		data["addStylishPointParry"] = addStylishPointOnParry;
		data["parryPadVibePower"] = parryPadVibePower;
		data["parryPadVibeTime"] = parryPadVibeTime;
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
		LoadJsonFloat("parryPadVibePower", parryPadVibePower, _data);
		LoadJsonFloat("parryPadVibeTime", parryPadVibeTime, _data);
		json loadData;
		if (LoadJsonData("vfxInfo", loadData, _data))
		{
			parryEffectInfo.Load(loadData);
		}

		if (LoadJsonData("parryCamShake", loadData, _data))
		{
			parryCamShakeParam.Load(loadData);
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

		// パリィを行うのかチェックする
		CheckPerformParry();

		//CP_RigidBody& rb = GetRB();
		//DXSimp::Vector3 velocity;
		//velocity.y = rb.GetVelocity().y;
		//GetRB().SetVelocity(velocity);
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

	void PlayerGuardState::CheckPerformParry()
	{
		// ダメージ処理時に行わない理由は攻撃ヒット判定と同時にガードすると、
		// パリィのアニメーションが行われなかったのでコールバック関数内ではなく更新処理内で行うようにする

		// 行わないなら処理しない
		if (!isPerformParryFrame) return;

		// パリィを繰り出す
		isPerformParryFrame = false;
		OnParry();
		
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

	void PlayerGuardState::CameraShakeOnParry()
	{
		if (!pCameraMove) // カメラ移動クラスがなければ
		{
			// カメラ移動クラスを取得
			pCameraMove =
				InSceneSystemManager::GetInstance()->GetMainCamera().GetGameObject().GetComponent<CP_CameraMove>();

			if (!pCameraMove) return;
		}

		// カメラシェイクする
		pCameraMove->ShakeCamera(parryCamShakeParam);
	}

	void PlayerGuardState::PadVibrationOnParry()
	{
		InSceneSystemManager::GetInstance()->GetInput().BeginVibration(parryPadVibePower, parryPadVibeTime);
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

		// カメラシェイク
		ImGuiMethod::LineSpaceSmall();
		parryCamShakeParam.ImGuiCall();

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

		// パッド振動
		ImGuiMethod::LineSpaceSmall();
		ImGui::Text("Pad Vibe");
		ImGui::DragFloat("VibePower", &parryPadVibePower, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("VibeTime", &parryPadVibeTime, 0.01f, 0.0f, 100.0f);
	}
}