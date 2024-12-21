#include "pch.h"
#include "PlayerGroundChargeAttack.h"
#include "GameObject.h"
#include "CameraChargeAttack.h"

namespace HashiTaku
{
	PlayerGroundChargeAttack::PlayerGroundChargeAttack() :
		pCamMove(nullptr),
		maxChargeTime(5.0f),
		curChargingTime(0.0f),
		curChargeLevel(ChargeLevel::Low),
		isCharging(false)
	{
		// 初期化
		u_int levelCnt = static_cast<u_int>(ChargeLevel::MaxNum);
		for (u_int c_i = 0; c_i < levelCnt; c_i++)
		{
			chargeTimes[c_i] = 1.0f;
		}

		chargeAtkInfos.resize(1);	// 最低1用意
	}

	json PlayerGroundChargeAttack::Save()
	{
		auto data = PlayerAttackState::Save();
		data["maxTime"] = maxChargeTime;
		SaveJsonVector3("chargeOffset", chargeVfxOffset, data);
		data["chargeVfx"] = onNextChargeVfx.Save();

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
				atkInfoDatas.push_back(chargeAtkInfos[a_i][l_i].Save());
			}

			// チャージ時間
			chargeData["time"] = chargeTimes[l_i];
			chargeInfoDatas.push_back(chargeData);
		}

		return data;
	}

	void PlayerGroundChargeAttack::Load(const json& _data)
	{
		PlayerAttackState::Load(_data);
		LoadJsonFloat("maxTime", maxChargeTime, _data);
		LoadJsonVector3("chargeOffset", chargeVfxOffset, _data);
		json vfxData;
		if (LoadJsonData("chargeVfx", vfxData, _data))
		{
			onNextChargeVfx.Load(vfxData);
		}

		json chargeDatas;
		if (!LoadJsonDataArray("chargeInfos", chargeDatas, _data)) return;
		int l_i = -1;
		for (auto& chargeData : chargeDatas)
		{
			l_i++;
			LoadJsonFloat("time", chargeTimes[l_i], chargeData);

			int a_i = -1;
			json attackInfoData;
			if (LoadJsonDataArray("attackInfos", attackInfoData, chargeData))
			{
				a_i++;

				// 配列が用意されていないなら
				if (static_cast<int>(chargeAtkInfos.size()) <= a_i) break;
				chargeAtkInfos[a_i][l_i].Load(attackInfoData[a_i]);
			}
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
		curChargeLevel = ChargeLevel::Low;

		// アニメーション側
		GetAnimation()->SetBool(CHARGE_PARAMNAME, true);	// チャージ中か
		GetAnimation()->SetInt(CHARGE_STEP_PARAMNAME, 1);	// チャージの段階
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

	void PlayerGroundChargeAttack::ChangeCameraChargeState()
	{
		// カメラ移動コンポーネント取得できていないなら
		if (!pCamMove)
		{
			// カメラ移動コンポーネント取得
			GameObject& cameraObj = pActionController->GetCamera().GetGameObject();
			pCamMove = cameraObj.GetComponent<CP_CameraMove>();

			if (!pCamMove) return;	// 取得できないなら
		}

		// カメラステートを変更
		pCamMove->ChangeState(CP_CameraMove::CameraState::Charge);
	}

	void PlayerGroundChargeAttack::OnChangeAttackTimes()
	{
		PlayerAttackState::OnChangeAttackTimes();

		u_int atktimes = GetAttackTimes();
		// サイズを変更
		chargeAtkInfos.resize(atktimes);
	}

	void PlayerGroundChargeAttack::CheckInputRelease()
	{
		// 入力上げたら
		if (pPlayerInput->GetButtonUp(GameInput::ButtonType::Player_ChargeAttack))
			OnChargeEnd();
	}

	void PlayerGroundChargeAttack::ChargingUpdate()
	{
		curChargingTime += DeltaTime();

		int nextLevelId = static_cast<int>(curChargeLevel) + 1;
		int maxLevelId = static_cast<int>(ChargeLevel::MaxNum);
		if (nextLevelId != maxLevelId)	// 最大レベルでない
		{
			// チャージ時間が来たら
			if (curChargingTime > chargeTimes[nextLevelId])
				NextChargeLevel();
		}

		// 最大時間が溜まれば
		if (curChargingTime > maxChargeTime)
			OnChargeEnd();
	}

	void PlayerGroundChargeAttack::NextChargeLevel()
	{
		// 次のレベルへ
		int nextLevelId = static_cast<int>(curChargeLevel) + 1;
		curChargeLevel = static_cast<ChargeLevel>(nextLevelId);
		GetAnimation()->SetInt(CHARGE_STEP_PARAMNAME, nextLevelId + 1);	// チャージの段階

		// エフェクトを出す
		DXSimp::Vector3 effectPos = GetMyTransform().GetPosition();
		DX11EffecseerManager::GetInstance()->Play(onNextChargeVfx, effectPos + chargeVfxOffset);

		HASHI_DEBUG_LOG("チャージ段階：" + std::string(magic_enum::enum_name(curChargeLevel)));
	}

	void PlayerGroundChargeAttack::OnChargeEnd()
	{
		HASHI_DEBUG_LOG("チャージ終了");

		isCharging = false;	// 溜め終了
		GetAnimation()->SetBool(CHARGE_PARAMNAME, false);

		// カメラを通常状態に戻す
		if (pCamMove)
		{
			CameraChargeAttack* pCamCharge = static_cast<CameraChargeAttack*>(
				pCamMove->GetState(CP_CameraMove::CameraState::Charge)
				);
			if (pCamCharge) pCamCharge->EndChargingShake();

			pCamMove->ChangeState(CP_CameraMove::CameraState::Move);
		}
			

		// 与えるダメージ量を求める（溜めた分だけダメージアップ）
		u_int atkTimes = GetAttackTimes();
		for (u_int a_i = 0; a_i < atkTimes; a_i++)
		{
			// 攻撃情報を取得
			auto* pAtkInfo = GetPlayerAtkInfo(a_i);
			if (!pAtkInfo) continue;

			// 攻撃ごとのダメージ情報を上書きする
			u_int chargeId = static_cast<u_int>(curChargeLevel);
			*pAtkInfo = chargeAtkInfos[a_i][chargeId];
		}
	}

	void PlayerGroundChargeAttack::ImGuiDebug()
	{
		ImGui::DragFloat("MaxTime", &maxChargeTime, 0.01f, 0.0f, 100.0f);

		if (ImGuiMethod::TreeNode("Charge Time"))
		{
			// チャージレベルごとの
			u_int chargeCnt = static_cast<u_int>(ChargeLevel::MaxNum);
			for (u_int c_i = 1; c_i < chargeCnt; c_i++)
			{
				ChargeLevel c = static_cast<ChargeLevel>(c_i);
				std::string levelStr = std::string(magic_enum::enum_name(c));
				ImGui::DragFloat(levelStr.c_str(), &chargeTimes[c_i], 0.01f, 0.0f, 100.0f);
			}

			ImGui::TreePop();
		}

		ImGui::DragFloat3("ChargeOffset", &chargeVfxOffset.x, 0.1f);
		onNextChargeVfx.ImGuiCall();

		ImGuiAttackInfo();
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
						chargeAtkInfos[a_i][l_i].ImGuiCall();
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
	}
}