#include "pch.h"
#include "BossAttackState.h"
#include "BossActionController.h"
#include "CP_Animation.h"

BossAttackState::BossAttackState()
{
	// 1つは用意する
	HashiTaku::AttackInformation attackInfo;
	attackInfos.push_back(attackInfo);
}

void BossAttackState::OnStartBehavior()
{
	BossActState_Base::OnStartBehavior();

	// 攻撃トリガー
	pActionController->GetAnimation()->SetTrigger(ATTACKTRIGGER_ANIMPARAM_NAME);

	// 攻撃情報をセットする
	pActionController->SetAttackInfo(attackInfos[0]);
}

void BossAttackState::UpdateBehavior()
{
	BossActState_Base::UpdateBehavior();
}

void BossAttackState::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	// 待機に戻ったら
	if (_toAnimNodeName == IDLE_ANIM_NAME)
		ChangeState(BossState::Idle);
}

nlohmann::json BossAttackState::Save()
{
	auto data = BossActState_Base::Save();

	// 攻撃情報をセーブ
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

	// 攻撃情報をロード
	nlohmann::json atkLoadDatas;
	if (HashiTaku::LoadJsonDataArray("atkInfos", atkLoadDatas, _data))
	{
		attackInfos.clear();	// 今の情報をリセット
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

	/// 攻撃情報
	u_int infoCnt = static_cast<u_int>(attackInfos.size());
	for (u_int a_i = 0; a_i < infoCnt; a_i++)
	{
		ImGui::PushID(a_i);

		std::string text = std::to_string(a_i) + "段目";
		ImGui::Text(text.c_str());
		attackInfos[a_i].ImGuiCall();
		ImGuiMethod::LineSpaceSmall();

		ImGui::PopID();
	}
}
