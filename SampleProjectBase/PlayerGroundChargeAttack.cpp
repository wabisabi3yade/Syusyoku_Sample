#include "pch.h"
#include "PlayerGroundChargeAttack.h"
#include "PlayerGroundActionController.h"
#include "GameObject.h"
#include "CameraChargeAttack.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	constexpr int NONE_VFX_HANDLE = -9999;	// エフェクトなしを表すエフェクトハンドル

	PlayerGroundChargeAttack::PlayerGroundChargeAttack() :
		pCamMove(nullptr),
		maxChargeTime(5.0f),
		curChargingTime(0.0f),
		lastChargingTime(0.0f),
		chargeVfxCreateTimeOffset(0.0f),
		chargeReleaseVfxOffsetY(0.0f),
		chargePadShakePower(0.4f),
		curChargeLevel(ChargeLevel::Low),
		chargeVfxHandle(NONE_VFX_HANDLE),
		isCharging(false)
	{
		for (auto& time : chargeTimes)
		{
			time = 0.0f;
		}
	}

	json PlayerGroundChargeAttack::Save()
	{
		auto data = PlayerAttackState::Save();
		data["maxTime"] = maxChargeTime;
		data["chargeOffsetTime"] = chargeVfxCreateTimeOffset;
		SaveJsonVector3("chargeOffset", chargeVfxOffset, data);
		data["releaseVfxOffset"] = chargeReleaseVfxOffsetY;
		data["charhePadPower"] = chargePadShakePower;

		// チャージ段階ごとの情報
		auto& chargeInfoDatas = data["chargeInfos"];
		u_int levelCnt = static_cast<u_int>(ChargeLevel::MaxNum);
		for (u_int l_i = 0; l_i < levelCnt; l_i++)
		{
			json chargeData;

			// 攻撃情報
			json& atkInfoDatas = chargeData["attackInfos"];
			u_int attackTimes = GetAttackTimes();
			for (u_int a_i = 0; a_i < attackTimes; a_i++)
			{
				atkInfoDatas.push_back(chargeAtkInfos[a_i][l_i]->Save());
			}

			// チャージ時間
			chargeData["time"] = chargeTimes[l_i];
			// エフェクト
			chargeData["nextVfx"] = chargeCompleteVfxs[l_i].Save();
			chargeData["releaseVfx"] = chargeReleaseVfxs[l_i].Save();
			chargeData["nextSound"] = chargeCompleteSounds[l_i].Save();
			chargeData["releaseSound"] = chargeReleaseSounds[l_i].Save();
			chargeInfoDatas.push_back(chargeData);
		}

		return data;
	}

	void PlayerGroundChargeAttack::Load(const json& _data)
	{
		PlayerAttackState::Load(_data);
		LoadJsonFloat("maxTime", maxChargeTime, _data);
		LoadJsonFloat("chargeOffsetTime", chargeVfxCreateTimeOffset, _data);
		LoadJsonFloat("charhePadPower", chargePadShakePower, _data);
		LoadJsonVector3("chargeOffset", chargeVfxOffset, _data);
		LoadJsonFloat("releaseVfxOffset", chargeReleaseVfxOffsetY, _data);
	

		json chargeDatas;
		if (!LoadJsonDataArray("chargeInfos", chargeDatas, _data)) return;
		int l_i = -1;
		for (auto& chargeData : chargeDatas)
		{
			l_i++;
			LoadJsonFloat("time", chargeTimes[l_i], chargeData);

			int a_i = -1;
			json chargeParamData;
			if (LoadJsonDataArray("attackInfos", chargeParamData, chargeData))
			{
				a_i++;

				// 配列が用意されていないなら
				if (static_cast<int>(chargeAtkInfos.size()) <= a_i) break;
				chargeAtkInfos[a_i][l_i]->Load(chargeParamData[a_i]);
			}
			if (LoadJsonData("nextVfx", chargeParamData, chargeData))
			{
				chargeCompleteVfxs[l_i].Load(chargeParamData);
			}
			if (LoadJsonData("releaseVfx", chargeParamData, chargeData))
			{
				chargeReleaseVfxs[l_i].Load(chargeParamData);
			}
			if (LoadJsonData("nextSound", chargeParamData, chargeData))
			{
				chargeCompleteSounds[l_i].Load(chargeParamData);
			}
			if (LoadJsonData("releaseSound", chargeParamData, chargeData))
			{
				chargeReleaseSounds[l_i].Load(chargeParamData);
			}
		}
	}

	void PlayerGroundChargeAttack::InitState()
	{
		chargeAtkInfos.resize(1);	// 最低1用意

		// 初期化
		u_int levelCnt = static_cast<u_int>(ChargeLevel::MaxNum);
		for (u_int c_i = 0; c_i < levelCnt; c_i++)
		{
			// 攻撃情報を初期化
			chargeAtkInfos[0][c_i] = CreateAttackInfo();

			chargeTimes[c_i] = 1.0f;
		}
	}

	void PlayerGroundChargeAttack::OnStartBehavior()
	{
		PlayerAttackState::OnStartBehavior();

		// チャージ状態のカメラに変更
		ChangeCameraChargeState();

		// パラメータ初期化
		isCharging = true;
		curChargingTime = 0.0f;
		lastChargingTime = -Mathf::epsilon;
		curChargeLevel = ChargeLevel::Low;

		// アニメーション側
		GetAnimation()->SetBool(CHARGE_PARAMNAME, true);	// チャージ中か
		GetAnimation()->SetInt(CHARGE_STEP_PARAMNAME, 1);	// チャージの段階

		// チャージ中にゲームパッド振動し続ける
		InSceneSystemManager::GetInstance()->GetInput().
			BeginVibration(chargePadShakePower, 1000.0f);
	}


	void PlayerGroundChargeAttack::UpdateBehavior()
	{
		PlayerAttackState::UpdateBehavior();

		if (isCharging)
		{
			// 入力離したか確認
			CheckInputRelease();

			// チャージ中の更新処理
			ChargingUpdate();
		}
	}

	void PlayerGroundChargeAttack::OnEndBehavior()
	{
		// チャージのエフェクトを消す
		DX11EffekseerManager::GetInstance()->DestroyVfx(chargeVfxHandle);

		// チャージ中からカメラを通常に戻す
		ChangeCameraNormaleState();

		// 振動を終える
		InSceneSystemManager::GetInstance()->GetInput().BeginVibration(0.0f, 0.0f);
	}

	void PlayerGroundChargeAttack::ChangeCameraChargeState()
	{
		// カメラ移動コンポーネント取得できていないなら
		if (!pCamMove)
		{
			// カメラ移動コンポーネント取得
			GameObject& cameraObj = GetGroundController().GetCamera().GetGameObject();
			pCamMove = cameraObj.GetComponent<CP_CameraMove>();

			if (!pCamMove) return;	// 取得できないなら
		}

		// カメラステートを変更
		pCamMove->ChangeState(CP_CameraMove::CameraState::Charge);
	}

	void PlayerGroundChargeAttack::ChangeCameraNormaleState()
	{
		// カメラを通常状態に戻す
		if (!pCamMove) return;

		CameraChargeAttack* pCamCharge = static_cast<CameraChargeAttack*>(
			pCamMove->GetState(CP_CameraMove::CameraState::Charge)
			);
		if (pCamCharge) pCamCharge->EndChargingShake();

		pCamMove->ChangeState(CP_CameraMove::CameraState::Follow);

	}

	void PlayerGroundChargeAttack::OnChangeAttackTimes()
	{
		PlayerAttackState::OnChangeAttackTimes();


		u_int atktimes = GetAttackTimes();
		u_int prevTimes = static_cast<u_int>(chargeAtkInfos.size());
		u_int chargeCnt = static_cast<u_int>(ChargeLevel::MaxNum);
		// サイズを変更
		chargeAtkInfos.resize(atktimes);

		// 追加した分の攻撃情報を生成
		for (; prevTimes < atktimes; prevTimes++)
		{
			for (u_int c_i = 0; c_i < chargeCnt; c_i++)
			{
				chargeAtkInfos[prevTimes][c_i] = CreateAttackInfo();
			}
		}
	}

	void PlayerGroundChargeAttack::CheckInputRelease()
	{
		// 入力していなかったら
		if (isCharging &&
			!pPlayerInput->GetButton(GameInput::ButtonType::Player_ChargeAttack))
			OnChargeEnd();
	}

	void PlayerGroundChargeAttack::ChargingUpdate()
	{
		curChargingTime += DeltaTime();

		int nextLevelId = static_cast<int>(curChargeLevel) + 1;
		int maxLevelId = static_cast<int>(ChargeLevel::MaxNum);
		if (nextLevelId != maxLevelId)	// 最大レベルでない
		{
			// エフェクトを出すタイミングになった瞬間なら
			if (curChargingTime > chargeTimes[nextLevelId] + chargeVfxCreateTimeOffset &&
				lastChargingTime < chargeTimes[nextLevelId] + chargeVfxCreateTimeOffset)
				CreateChargeVfx(static_cast<ChargeLevel>(nextLevelId));

			// チャージ時間が来たら
			if (curChargingTime > chargeTimes[nextLevelId])
				NextChargeLevel();
		}

		// 最大時間が溜まれば
		if (curChargingTime > maxChargeTime)
			OnChargeEnd();

		lastChargingTime = curChargingTime;

	}

	void PlayerGroundChargeAttack::CreateChargeVfx(ChargeLevel _chargeLevel)
	{
		// エフェクトを出す
		DXSimp::Vector3 effectPos = GetMyTransform().GetPosition();
		chargeVfxHandle = DX11EffekseerManager::GetInstance()->Play(
			chargeCompleteVfxs[static_cast<int>(_chargeLevel)],
			effectPos + chargeVfxOffset);
	}

	void PlayerGroundChargeAttack::NextChargeLevel()
	{
		// 次のレベルへ
		int nextLevelId = static_cast<int>(curChargeLevel) + 1;
		curChargeLevel = static_cast<ChargeLevel>(nextLevelId);
		GetAnimation()->SetInt(CHARGE_STEP_PARAMNAME, nextLevelId + 1);	// チャージの段階

		// チャージのエフェクトのハンドルをリセット
		chargeVfxHandle = NONE_VFX_HANDLE;

		// チャージ完了サウンドを再生
		const DXSimp::Vector3& playerPos = GetMyTransform().GetPosition();
		CreateSoundFX(chargeCompleteSounds[nextLevelId], playerPos);
	}

	void PlayerGroundChargeAttack::OnChargeEnd()
	{
		isCharging = false;	// 溜め終了
		GetAnimation()->SetBool(CHARGE_PARAMNAME, false);

		// パッド振動をやめる
		InSceneSystemManager::GetInstance()->GetInput().BeginVibration(0.0f, 0.0f);

		// カメラを通常に戻す
		ChangeCameraNormaleState();

		// 与えるダメージ量を求める（溜めた分だけダメージアップ）
		u_int atkTimes = GetAttackTimes();
		for (u_int a_i = 0; a_i < atkTimes; a_i++)
		{
			// 攻撃情報を取得
			auto* pAtkInfo = GetPlayerAtkInfo(a_i);
			if (!pAtkInfo) continue;

			// 攻撃ごとのダメージ情報を上書きする
			u_int chargeId = static_cast<u_int>(curChargeLevel);
			*pAtkInfo = *chargeAtkInfos[a_i][chargeId];
		}

		// チャージエフェクトを消す
		auto* pVfxManager = DX11EffekseerManager::GetInstance();
		if (chargeVfxHandle != NONE_VFX_HANDLE)
			pVfxManager->DestroyVfx(chargeVfxHandle);

		// 攻撃移行時のエフェクトを出す
		// 座標
		DXSimp::Vector3 vfxPos = GetMyTransform().GetPosition() +
			Vec3::Up * chargeReleaseVfxOffsetY;
		pVfxManager->Play(chargeReleaseVfxs[static_cast<int>(curChargeLevel)],
			vfxPos);

		// 解放サウンド再生
		CreateSoundFX(chargeReleaseSounds[static_cast<int>(curChargeLevel)],
			vfxPos);
	}

	void PlayerGroundChargeAttack::ImGuiDebug()
	{
		ImGui::DragFloat("MaxTime", &maxChargeTime, 0.01f, 0.0f, 100.0f);

		// チャージレベルごとの編集
		u_int chargeCnt = static_cast<u_int>(ChargeLevel::MaxNum);
		for (u_int c_i = 0; c_i < chargeCnt; c_i++)
		{
			ChargeLevel c = static_cast<ChargeLevel>(c_i);
			std::string levelStr = std::string(magic_enum::enum_name(c));

			if (!ImGuiMethod::TreeNode(levelStr)) continue;

			// チャージ時間
			ImGui::DragFloat("Charge Time", &chargeTimes[c_i], 0.01f, 0.0f, 100.0f);

			// チャージしたときのvfx
			ImGui::PushID(0);
			ImGui::Text("Complete");
			chargeCompleteVfxs[c_i].ImGuiCall();
			if (ImGuiMethod::TreeNode("Sound"))
			{
				chargeCompleteSounds[c_i].ImGuiCall();
				ImGui::TreePop();
			}
			ImGui::PopID();

			ImGuiMethod::LineSpaceSmall();

			// 解放したときのvfx
			ImGui::PushID(1);
			ImGui::Text("Release");
			chargeReleaseVfxs[c_i].ImGuiCall();
			if (ImGuiMethod::TreeNode("Sound"))
			{
				chargeReleaseSounds[c_i].ImGuiCall();
				ImGui::TreePop();
			}
			ImGui::PopID();

			ImGuiMethod::LineSpaceSmall();

			ImGui::TreePop();
		}

		ImGui::PushID("ChargeVfx");
		ImGui::Text("ChargeVfx");
		ImGui::DragFloat("ChargeOffsetTime", &chargeVfxCreateTimeOffset, 0.01f);
		ImGui::DragFloat3("ChargeOffsetPos", &chargeVfxOffset.x, 0.1f);
		ImGui::PopID();
		ImGuiMethod::LineSpaceSmall();
		ImGui::Text("ChargeReleaseVfx");
		ImGui::DragFloat("ReleaseOffsetY", &chargeReleaseVfxOffsetY, 0.1f);

		ImGuiAttackInfo();

		ImGui::DragFloat("PadPower", &chargePadShakePower, 0.1f, 0.0f, 1.0f);
	}

	void PlayerGroundChargeAttack::ImGuiAttackInfo()
	{
		// 攻撃回数
		u_int attackTimes = GetAttackTimes();
		for (u_int a_i = 0; a_i < attackTimes; a_i++)
		{
			if (ImGuiMethod::TreeNode("Step:" + std::to_string(a_i + 1)))
			{
				// チャージレベルごとの
				u_int chargeCnt = static_cast<u_int>(ChargeLevel::MaxNum);
				for (u_int l_i = 0; l_i < chargeCnt; l_i++)
				{
					ChargeLevel c = static_cast<ChargeLevel>(l_i);
					std::string levelStr = std::string(magic_enum::enum_name(c));
					if (ImGuiMethod::TreeNode(levelStr))
					{
						chargeAtkInfos[a_i][l_i]->ImGuiCall();
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
	}
}