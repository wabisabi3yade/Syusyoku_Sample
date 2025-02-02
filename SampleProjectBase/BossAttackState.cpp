#include "pch.h"
#include "BossAttackState.h"
#include "BossActionController.h"
#include "CP_Animation.h"
#include "CP_Boss.h"


namespace HashiTaku
{
	BossAttackState::BossAttackState() :
		rotSpeedTimes(1.0f),
		attackTimeCnt(1),
		curAttackTime(1),
		isUseRotateCurve(false)
	{
		// 1�͗p�ӂ���
		attackInfos.push_back(CreateAttackInfo());

#ifdef EDIT
		rotSpeedCurve.SetCurveName("Rotate Speed");
#endif // EDIT
	}

	void BossAttackState::OnStartBehavior()
	{
		BossActState_Base::OnStartBehavior();

		// �U���g���K�[
		pActionController->GetAnimation()->SetTrigger(ATTACKTRIGGER_ANIMPARAM_NAME);

		// �U�������Z�b�g����
		pActionController->SetAttackInfo(*attackInfos[0]);

		// ������
		curAttackTime = 1;	// 1�i�ڂ������

		// �v���C���[�߂����ă��[�v����悤�ɂ���
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
		// �ҋ@�ɖ߂�����
		if (_toAnimNodeName == IDLE_ANIM_NAME)
			ChangeState(BossState::Idle);
	}

	json BossAttackState::Save()
	{
		auto data = BossActState_Base::Save();

		data["atkTimeCnt"] = attackTimeCnt;

		// �U�������Z�[�u
		auto& atkInfoData = data["atkInfos"];
		for (auto& saveAtkInfo : attackInfos)
		{
			atkInfoData.push_back(saveAtkInfo->Save());
		}

		data["useRotCurve"] = isUseRotateCurve;
		if (isUseRotateCurve)
		{
			data["rotTimes"] = rotSpeedTimes;
			data["rotCurve"] = rotSpeedCurve.Save();
		}


		return data;
	}

	void BossAttackState::Load(const json& _data)
	{
		BossActState_Base::Load(_data);

		LoadJsonUnsigned("atkTimeCnt", attackTimeCnt, _data);

		// �U���������[�h
		json atkLoadDatas;
		if (LoadJsonDataArray("atkInfos", atkLoadDatas, _data))
		{
			attackInfos.clear();	// ���̏������Z�b�g
			for (auto& atkLoadData : atkLoadDatas)
			{
				std::unique_ptr<BossAttackInformation> pAtkInfo = CreateAttackInfo();
				pAtkInfo->Load(atkLoadData);
				attackInfos.push_back(std::move(pAtkInfo));
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

		// �v���C���[�ւ̃x�N�g�������߂�
		DXSimp::Vector3 vecToPlayer = GetPlayerTransform().GetPosition() - bossTransform.GetPosition();
		vecToPlayer.y = 0.0f;
		vecToPlayer.Normalize();
		DXSimp::Quaternion targetRot = Quat::RotateToVector(vecToPlayer);

		// ��]
		DXSimp::Quaternion bossRot = bossTransform.GetRotation();
		bossRot = DXSimp::Quaternion::Slerp(bossRot, targetRot, rotSpeed * rotSpeedTimes * DeltaTime());
		bossTransform.SetRotation(bossRot);
	}

	std::unique_ptr<BossAttackInformation> BossAttackState::CreateAttackInfo()
	{			
		// �U�����𐶐�
		return std::make_unique<BossAttackInformation>(&pActionController->GetBoss());
	}

	void BossAttackState::ImGuiDebug()
	{
		BossActState_Base::ImGuiDebug();

		ImGuiMethod::LineSpaceSmall();

		/// �U�����
		ImGui::PushID("Attack");
		ImGui::Text("Attack");
		u_int infoCnt = static_cast<u_int>(attackInfos.size());
		for (u_int a_i = 0; a_i < infoCnt; a_i++)
		{
			std::string caption = "Step:" + std::to_string(a_i + 1);
			if(!ImGui::TreeNode(caption.c_str())) continue;

			std::string text = "Step" + std::to_string(a_i);
			ImGui::Text(text.c_str());
			attackInfos[a_i]->ImGuiCall();
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

		// �U����
		int imInt = static_cast<u_int>(attackTimeCnt);
		if (ImGui::DragInt("attackCnt", &imInt, 1, 1, 100))
		{
			SetAttackTimeCnt(imInt);
		}
	}

	void BossAttackState::UpdateReAttack()
	{
		// ���A�^�b�N�̃^�C�~���O�łȂ��Ȃ�
		if (!(pActionController->GetReAttack())) return;

		// ���A�^�b�N�t���O���~�낷
		GetAnimation()->SetBool(REATTACK_PARAMNAME, false);

		// ���A�^�b�N�X�V
		curAttackTime++;

		// ���U���񐔂𒴂���Ȃ�
		if (curAttackTime > attackTimeCnt)
		{
			HASHI_DEBUG_LOG("�G���[�F���U���񐔂𒴂��Ă��܂�");
			curAttackTime = attackTimeCnt;
		}

		assert(static_cast<u_int>(attackInfos.size()) >= curAttackTime &&
			"�U����񂪍U���񐔈ȉ��ł�");

		// �Ή������U�������Z�b�g����
		pActionController->SetAttackInfo(*attackInfos[curAttackTime - 1]);
	}

	void BossAttackState::SetAttackTimeCnt(u_int _attackTime)
	{
#ifdef EDIT
		if (_attackTime == 0)
		{
			HASHI_DEBUG_LOG("1�ȏ��ݒ肵�Ă�������");
			return;
		}
#endif // EDIT

		u_int prevAttackCnt = static_cast<u_int>(attackInfos.size());
		attackInfos.resize(_attackTime);	// �U���������킹��

		// �����������A�U�������𐶐�
		for (; prevAttackCnt < _attackTime; prevAttackCnt++)
		{
			attackInfos[prevAttackCnt] = CreateAttackInfo();
		}
	}
}