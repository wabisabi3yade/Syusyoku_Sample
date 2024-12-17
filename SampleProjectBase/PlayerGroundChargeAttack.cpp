#include "pch.h"
#include "PlayerGroundChargeAttack.h"

namespace HashiTaku
{
	PlayerGroundChargeAttack::PlayerGroundChargeAttack() :
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

	nlohmann::json PlayerGroundChargeAttack::Save()
	{
		auto data = PlayerAttackState::Save();
		data["maxTime"] = maxChargeTime;

		// チャージ段階ごとの情報
		auto& chargeInfoDatas = data["chargeInfos"];
		u_int levelCnt = static_cast<u_int>(ChargeLevel::MaxNum);
		for (u_int l_i = 0; l_i < levelCnt; l_i++)
		{
			nlohmann::json chargeData;

			// 攻撃情報
			nlohmann::json& atkInfoDatas = chargeData["attackInfos"];
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

	void PlayerGroundChargeAttack::Load(const nlohmann::json& _data)
	{
		PlayerAttackState::Load(_data);
		LoadJsonFloat("maxTime", maxChargeTime, _data);

		nlohmann::json chargeDatas;
		if (!LoadJsonDataArray("chargeInfos", chargeDatas, _data)) return;

		int l_i = -1;
		for (auto& chargeData : chargeDatas)
		{
			l_i++;
			LoadJsonFloat("time", chargeTimes[l_i], chargeData);

			int a_i = -1;
			nlohmann::json attackInfoData;
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

		// パラメータ初期化
		isCharging = true;
		curChargingTime = 0.0f;
		curChargeLevel = ChargeLevel::Low;

		// アニメーション側
		GetAnimation()->SetBool(CHARGE_PARAMNAME, true);
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

		// 最大時間が溜まれば
		if (curChargingTime > maxChargeTime)
			OnChargeEnd();
	}

	void PlayerGroundChargeAttack::OnChargeEnd()
	{
		isCharging = false;	// 溜め終了
		GetAnimation()->SetBool(CHARGE_PARAMNAME, false);

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

		if(ImGuiMethod::TreeNode("Charge Time"))
		{
			// チャージレベルごとの
			u_int chargeCnt = static_cast<u_int>(ChargeLevel::MaxNum);
			for (u_int c_i = 0; c_i < chargeCnt; c_i++)
			{
				ChargeLevel c = static_cast<ChargeLevel>(c_i);
				std::string levelStr = std::string(magic_enum::enum_name(c));
				ImGui::DragFloat(levelStr.c_str(), &chargeTimes[c_i], 0.01f, 0.0f, 100.0f);
			}

			ImGui::TreePop();
		}

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