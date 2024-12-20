#include "pch.h"
#include "BossAttackState.h"
#include "BossActionController.h"
#include "CP_Animation.h"


namespace HashiTaku
{
	BossAttackState::BossAttackState() :
		rotSpeedTimes(1.0f),
		attackTimeCnt(1),
		curAttackTime(1),
		isUseRotateCurve(false)
	{
		// 1つは用意する
		AttackInformation attackInfo;
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

		// 初期化
		curAttackTime = 1;	// 1段目から入る

		// プレイヤーめがけてワープするようにする
		SetWarpTargetPosReference(GetPlayerTransform().GetPosition());
	}

	void BossAttackState::UpdateBehavior()
	{
		UpdateReAttack();

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

		data["atkTimeCnt"] = attackTimeCnt;

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

		LoadJsonUnsigned("atkTimeCnt", attackTimeCnt, _data);

		// 攻撃情報をロード
		nlohmann::json atkLoadDatas;
		if (LoadJsonDataArray("atkInfos", atkLoadDatas, _data))
		{
			attackInfos.clear();	// 今の情報をリセット
			for (auto& atkLoadData : atkLoadDatas)
			{
				AttackInformation atkInfo;
				atkInfo.Load(atkLoadData);
				attackInfos.push_back(atkInfo);
			}
		}

		LoadJsonBoolean("useRotCurve", isUseRotateCurve, _data);
		if (isUseRotateCurve)
		{
			LoadJsonFloat("rotTimes", rotSpeedTimes, _data);

			if (IsJsonContains(_data, "rotCurve"))
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

		ImGuiMethod::LineSpaceSmall();

		/// 攻撃情報
		ImGui::PushID("Attack");
		ImGui::Text("Attack");
		u_int infoCnt = static_cast<u_int>(attackInfos.size());
		for (u_int a_i = 0; a_i < infoCnt; a_i++)
		{
			std::string caption = "Step:" + std::to_string(a_i + 1);
			if(!ImGui::TreeNode(caption.c_str())) continue;

			std::string text = "Step" + std::to_string(a_i);
			ImGui::Text(text.c_str());
			attackInfos[a_i].ImGuiCall();
			ImGuiMethod::LineSpaceSmall();

			ImGui::TreePop();
		}
		ImGui::PopID();

		ImGuiMethod::LineSpaceSmall();

		ImGui::Checkbox("UseRotateCurve", &isUseRotateCurve);
		if (isUseRotateCurve)
		{
			ImGui::DragFloat("Times", &rotSpeedTimes, 0.01f, 0.0f, 10.0f);
			rotSpeedCurve.ImGuiCall();
		}

		// 攻撃回数
		int imInt = static_cast<u_int>(attackTimeCnt);
		if (ImGui::DragInt("attackCnt", &imInt, 1, 1, 100))
		{
			SetAttackTimeCnt(imInt);
		}
	}

	void BossAttackState::UpdateReAttack()
	{
		// リアタックのタイミングでないなら
		if (!(pActionController->GetReAttack())) return;

		// リアタックフラグを降ろす
		GetAnimation()->SetBool(REATTACK_PARAMNAME, false);

		// リアタック更新
		curAttackTime++;

		// 総攻撃回数を超えるなら
		if (curAttackTime > attackTimeCnt)
		{
			HASHI_DEBUG_LOG("エラー：総攻撃回数を超えています");
			curAttackTime = attackTimeCnt;
		}

		assert(static_cast<u_int>(attackInfos.size()) >= curAttackTime &&
			"攻撃情報が攻撃回数以下です");

		// 対応した攻撃情報をセットする
		pActionController->SetAttackInfo(attackInfos[curAttackTime - 1]);
	}

	void BossAttackState::SetAttackTimeCnt(u_int _attackTime)
	{
#ifdef EDIT
		if (_attackTime == 0)
		{
			HASHI_DEBUG_LOG("1以上を設定してください");
			return;
		}
#endif // EDIT

		attackTimeCnt = _attackTime;
		attackInfos.resize(_attackTime);	// 攻撃情報も合わせる
	}
}