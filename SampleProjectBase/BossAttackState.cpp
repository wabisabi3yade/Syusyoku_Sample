#include "pch.h"
#include "BossAttackState.h"
#include "BossActionController.h"
#include "CP_Animation.h"

namespace DXSimp = DirectX::SimpleMath;

BossAttackState::BossAttackState() : rotSpeedTimes(1.0f), isUseRotateCurve(false)
{
	// 1つは用意する
	HashiTaku::AttackInformation attackInfo;
	attackInfos.push_back(attackInfo);

#ifdef EDIT
	rotSpeedCurve.SetCurveName("Rotate Speed");
#endif // EDIT
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
	RotateUpdate();

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

	data["useRotCurve"] = isUseRotateCurve;
	if (isUseRotateCurve)
	{
		data["rotTimes"] = rotSpeedTimes;
		data["rotCurve"] = rotSpeedCurve.Save();
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

	HashiTaku::LoadJsonBoolean("useRotCurve", isUseRotateCurve, _data);
	if (isUseRotateCurve)
	{
		HashiTaku::LoadJsonFloat("rotTimes", rotSpeedTimes, _data);

		if (HashiTaku::IsJsonContains(_data, "rotCurve"))
			rotSpeedCurve.Load(_data["rotCurve"]);
	}
}

void BossAttackState::RotateUpdate()
{
	if (!isUseRotateCurve) return;
	float animRatio = GetAnimation()->GetCurrentPlayRatio();
	Transform& bossTransform = GetBossTransform();

	float rotSpeed = rotSpeedCurve.GetValue(animRatio);

	// プレイヤーへのベクトルを求める
	DXSimp::Vector3 vecToPlayer = GetPlayerTransform().GetPosition() - bossTransform.GetPosition();
	vecToPlayer.y = 0.0f;
	vecToPlayer.Normalize();
	DXSimp::Quaternion targetRot = Quat::RotateToVector(vecToPlayer);

	// 回転
	DXSimp::Quaternion bossRot = bossTransform.GetRotation();
	bossRot = DXSimp::Quaternion::Slerp(bossRot, targetRot, rotSpeed * rotSpeedTimes * DeltaTime());
	bossTransform.SetRotation(bossRot);
}

void BossAttackState::ImGuiDebug()
{
	BossActState_Base::ImGuiDebug();

	/// 攻撃情報
	u_int infoCnt = static_cast<u_int>(attackInfos.size());
	for (u_int a_i = 0; a_i < infoCnt; a_i++)
	{
		ImGui::PushID(a_i);

		std::string text = "Step" + std::to_string(a_i);
		ImGui::Text(text.c_str());
		attackInfos[a_i].ImGuiCall();
		ImGuiMethod::LineSpaceSmall();

		ImGui::PopID();
	}

	ImGuiMethod::LineSpaceSmall();

	ImGui::Checkbox("UseRotateCurve", &isUseRotateCurve);
	if (isUseRotateCurve)
	{
		ImGui::DragFloat("Times", &rotSpeedTimes, 0.01f, 0.0f, 10.0f);
		rotSpeedCurve.ImGuiCall();
	}

}