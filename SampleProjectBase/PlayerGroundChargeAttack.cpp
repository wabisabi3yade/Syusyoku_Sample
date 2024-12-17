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
		// ������
		u_int levelCnt = static_cast<u_int>(ChargeLevel::MaxNum);
		for (u_int c_i = 0; c_i < levelCnt; c_i++)
		{
			chargeTimes[c_i] = 1.0f;
		}

		chargeAtkInfos.resize(1);	// �Œ�1�p��
	}

	nlohmann::json PlayerGroundChargeAttack::Save()
	{
		auto data = PlayerAttackState::Save();
		data["maxTime"] = maxChargeTime;

		// �`���[�W�i�K���Ƃ̏��
		auto& chargeInfoDatas = data["chargeInfos"];
		u_int levelCnt = static_cast<u_int>(ChargeLevel::MaxNum);
		for (u_int l_i = 0; l_i < levelCnt; l_i++)
		{
			nlohmann::json chargeData;

			// �U�����
			nlohmann::json& atkInfoDatas = chargeData["attackInfos"];
			u_int attackTimes = GetAttackTimes();
			for (u_int a_i = 0; a_i < attackTimes; a_i++)
			{
				atkInfoDatas.push_back(chargeAtkInfos[a_i][l_i].Save());
			}

			// �`���[�W����
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

				// �z�񂪗p�ӂ���Ă��Ȃ��Ȃ�
				if (static_cast<int>(chargeAtkInfos.size()) <= a_i) break;
				chargeAtkInfos[a_i][l_i].Load(attackInfoData[a_i]);
			}
		}
	}

	void PlayerGroundChargeAttack::OnStartBehavior()
	{
		PlayerAttackState::OnStartBehavior();

		// �p�����[�^������
		isCharging = true;
		curChargingTime = 0.0f;
		curChargeLevel = ChargeLevel::Low;

		// �A�j���[�V������
		GetAnimation()->SetBool(CHARGE_PARAMNAME, true);
	}


	void PlayerGroundChargeAttack::UpdateBehavior()
	{
		PlayerAttackState::UpdateBehavior();

		if (isCharging)
		{
			// ���͗��������m�F
			CheckInputRelease();

			// �`���[�W���̍X�V����
			ChargingUpdate();
		}
	}

	void PlayerGroundChargeAttack::OnChangeAttackTimes()
	{
		PlayerAttackState::OnChangeAttackTimes();

		u_int atktimes = GetAttackTimes();
		// �T�C�Y��ύX
		chargeAtkInfos.resize(atktimes);
	}

	void PlayerGroundChargeAttack::CheckInputRelease()
	{
		// ���͏グ����
		if (pPlayerInput->GetButtonUp(GameInput::ButtonType::Player_ChargeAttack))
			OnChargeEnd();
	}

	void PlayerGroundChargeAttack::ChargingUpdate()
	{
		curChargingTime += DeltaTime();

		// �ő厞�Ԃ����܂��
		if (curChargingTime > maxChargeTime)
			OnChargeEnd();
	}

	void PlayerGroundChargeAttack::OnChargeEnd()
	{
		isCharging = false;	// ���ߏI��
		GetAnimation()->SetBool(CHARGE_PARAMNAME, false);

		// �^����_���[�W�ʂ����߂�i���߂��������_���[�W�A�b�v�j
		u_int atkTimes = GetAttackTimes();
		for (u_int a_i = 0; a_i < atkTimes; a_i++)
		{
			// �U�������擾
			auto* pAtkInfo = GetPlayerAtkInfo(a_i);
			if (!pAtkInfo) continue;

			// �U�����Ƃ̃_���[�W�����㏑������
			u_int chargeId = static_cast<u_int>(curChargeLevel);
			*pAtkInfo = chargeAtkInfos[a_i][chargeId];
		}
	}

	void PlayerGroundChargeAttack::ImGuiDebug()
	{
		ImGui::DragFloat("MaxTime", &maxChargeTime, 0.01f, 0.0f, 100.0f);

		if(ImGuiMethod::TreeNode("Charge Time"))
		{
			// �`���[�W���x�����Ƃ�
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
		// �U����
		u_int attackTimes = GetAttackTimes();
		for (u_int a_i = 0; a_i < attackTimes; a_i++)
		{
			if (ImGuiMethod::TreeNode("Step:" + std::to_string(a_i + 1)))
			{
				// �`���[�W���x�����Ƃ�
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