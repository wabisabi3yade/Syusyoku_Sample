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
		// ������
		u_int levelCnt = static_cast<u_int>(ChargeLevel::MaxNum);
		for (u_int c_i = 0; c_i < levelCnt; c_i++)
		{
			chargeTimes[c_i] = 1.0f;
		}

		chargeAtkInfos.resize(1);	// �Œ�1�p��
	}

	json PlayerGroundChargeAttack::Save()
	{
		auto data = PlayerAttackState::Save();
		data["maxTime"] = maxChargeTime;
		SaveJsonVector3("chargeOffset", chargeVfxOffset, data);
		data["chargeVfx"] = onNextChargeVfx.Save();

		// �`���[�W�i�K���Ƃ̏��
		auto& chargeInfoDatas = data["chargeInfos"];
		u_int levelCnt = static_cast<u_int>(ChargeLevel::MaxNum);
		for (u_int l_i = 0; l_i < levelCnt; l_i++)
		{
			json chargeData;

			// �U�����
			json& atkInfoDatas = chargeData["attackInfos"];
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

				// �z�񂪗p�ӂ���Ă��Ȃ��Ȃ�
				if (static_cast<int>(chargeAtkInfos.size()) <= a_i) break;
				chargeAtkInfos[a_i][l_i].Load(attackInfoData[a_i]);
			}
		}
	}

	void PlayerGroundChargeAttack::OnStartBehavior()
	{
		PlayerAttackState::OnStartBehavior();

		// �`���[�W��Ԃ̃J�����ɕύX
		ChangeCameraChargeState();

		// �p�����[�^������
		isCharging = true;
		curChargingTime = 0.0f;
		curChargeLevel = ChargeLevel::Low;

		// �A�j���[�V������
		GetAnimation()->SetBool(CHARGE_PARAMNAME, true);	// �`���[�W����
		GetAnimation()->SetInt(CHARGE_STEP_PARAMNAME, 1);	// �`���[�W�̒i�K
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

	void PlayerGroundChargeAttack::ChangeCameraChargeState()
	{
		// �J�����ړ��R���|�[�l���g�擾�ł��Ă��Ȃ��Ȃ�
		if (!pCamMove)
		{
			// �J�����ړ��R���|�[�l���g�擾
			GameObject& cameraObj = pActionController->GetCamera().GetGameObject();
			pCamMove = cameraObj.GetComponent<CP_CameraMove>();

			if (!pCamMove) return;	// �擾�ł��Ȃ��Ȃ�
		}

		// �J�����X�e�[�g��ύX
		pCamMove->ChangeState(CP_CameraMove::CameraState::Charge);
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

		int nextLevelId = static_cast<int>(curChargeLevel) + 1;
		int maxLevelId = static_cast<int>(ChargeLevel::MaxNum);
		if (nextLevelId != maxLevelId)	// �ő僌�x���łȂ�
		{
			// �`���[�W���Ԃ�������
			if (curChargingTime > chargeTimes[nextLevelId])
				NextChargeLevel();
		}

		// �ő厞�Ԃ����܂��
		if (curChargingTime > maxChargeTime)
			OnChargeEnd();
	}

	void PlayerGroundChargeAttack::NextChargeLevel()
	{
		// ���̃��x����
		int nextLevelId = static_cast<int>(curChargeLevel) + 1;
		curChargeLevel = static_cast<ChargeLevel>(nextLevelId);
		GetAnimation()->SetInt(CHARGE_STEP_PARAMNAME, nextLevelId + 1);	// �`���[�W�̒i�K

		// �G�t�F�N�g���o��
		DXSimp::Vector3 effectPos = GetMyTransform().GetPosition();
		DX11EffecseerManager::GetInstance()->Play(onNextChargeVfx, effectPos + chargeVfxOffset);

		HASHI_DEBUG_LOG("�`���[�W�i�K�F" + std::string(magic_enum::enum_name(curChargeLevel)));
	}

	void PlayerGroundChargeAttack::OnChargeEnd()
	{
		HASHI_DEBUG_LOG("�`���[�W�I��");

		isCharging = false;	// ���ߏI��
		GetAnimation()->SetBool(CHARGE_PARAMNAME, false);

		// �J������ʏ��Ԃɖ߂�
		if (pCamMove)
		{
			CameraChargeAttack* pCamCharge = static_cast<CameraChargeAttack*>(
				pCamMove->GetState(CP_CameraMove::CameraState::Charge)
				);
			if (pCamCharge) pCamCharge->EndChargingShake();

			pCamMove->ChangeState(CP_CameraMove::CameraState::Move);
		}
			

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

		if (ImGuiMethod::TreeNode("Charge Time"))
		{
			// �`���[�W���x�����Ƃ�
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