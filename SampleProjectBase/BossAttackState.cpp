#include "pch.h"
#include "BossAttackState.h"
#include "BossActionController.h"
#include "CP_Animation.h"

BossAttackState::BossAttackState()
{
	// 1�͗p�ӂ���
	HashiTaku::AttackInformation attackInfo;
	attackInfos.push_back(attackInfo);
}

void BossAttackState::OnStartBehavior()
{
	BossActState_Base::OnStartBehavior();

	// �U���g���K�[
	pActionController->GetAnimation()->SetTrigger(ATTACKTRIGGER_ANIMPARAM_NAME);

	// �U�������Z�b�g����
	pActionController->SetAttackInfo(attackInfos[0]);
}

void BossAttackState::UpdateBehavior()
{
	BossActState_Base::UpdateBehavior();
}

void BossAttackState::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	// �ҋ@�ɖ߂�����
	if (_toAnimNodeName == IDLE_ANIM_NAME)
		ChangeState(BossState::Idle);
}

nlohmann::json BossAttackState::Save()
{
	auto data = BossActState_Base::Save();

	// �U�������Z�[�u
	auto& atkInfoData = data["atkInfos"];
	for (auto& saveAtkInfo : attackInfos)
	{
		atkInfoData.push_back(saveAtkInfo.Save());
	}

	return data;
}

void BossAttackState::Load(const nlohmann::json& _data)
{
	BossActState_Base::Load(_data);

	// �U���������[�h
	nlohmann::json atkLoadDatas;
	if (HashiTaku::LoadJsonDataArray("atkInfos", atkLoadDatas, _data))
	{
		attackInfos.clear();	// ���̏������Z�b�g
		for (auto& atkLoadData : atkLoadDatas)
		{
			HashiTaku::AttackInformation atkInfo;
			atkInfo.Load(atkLoadData);
			attackInfos.push_back(atkInfo);
		}
	}
}

void BossAttackState::ImGuiDebug()
{
	BossActState_Base::ImGuiDebug();

	/// �U�����
	u_int infoCnt = static_cast<u_int>(attackInfos.size());
	for (u_int a_i = 0; a_i < infoCnt; a_i++)
	{
		ImGui::PushID(a_i);

		std::string text = std::to_string(a_i) + "�i��";
		ImGui::Text(text.c_str());
		attackInfos[a_i].ImGuiCall();
		ImGuiMethod::LineSpaceSmall();

		ImGui::PopID();
	}
}
