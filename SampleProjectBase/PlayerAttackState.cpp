#include "pch.h"
#include "PlayerAttackState.h"
#include "PlayerGroundActionController.h"
#include "GameObject.h"

namespace HashiTaku
{
	// �u���Ɍ���̂𔽉f����G�Ƃ̋���
	constexpr float INSTANTLOOK_DISTANCE(8.0f);

	PlayerAttackState::PlayerAttackState() :
		pIsReAttack(nullptr),
		nextCombAtkState(PlayerState::None),
		atkMaxDistance(10.0f),
		curAtkProgressDis(0.0f),
		prevProgressDistance(0.0f),
		curAttackTime(1),
		attackTimeCnt(1),
		isMoveForward(false)
	{

		attackInfos.resize(1);	// �U�������Œ�1�쐬���Ă���
		PlayerAttackInformation atkInfo;
		attackInfos.push_back(atkInfo);

		// �J�[�u�����Z�b�g
		progressDistanceCurve.SetCurveName("AtkMoveSpd");
	}

	void PlayerAttackState::OnStartBehavior()
	{
		if (!pIsReAttack)
			pIsReAttack = GetAnimation()->GetParameterPointer<bool>(REATTACK_PARAMNAME);

		// �G�̍��W���擾
		DXSimp::Vector3 atkPos = GetAtkEnemyPos();

		// �i�ދ���
		curAtkProgressDis = atkMaxDistance;

		// ���������ēG�֌����킹�邩���f
		DXSimp::Vector3 vecToEnemy = atkPos - GetMyTransform().GetPosition(); vecToEnemy.y = 0.0f;
		if (pActionController->GetIsTargeting() || vecToEnemy.Length() < INSTANTLOOK_DISTANCE)
		{
			// �i�ދ��������߂�
			CalcProgressDis(atkPos);

			// �G�̕���������
			LookAtEnemyInstant(atkPos);
		}

		// �U�������X�V
		UpdateAttackInfo();

		// �U���t���O�𗧂Ă�
		pActionController->SetAnimationTrigger(ATTACKTRIGGER_PARAMNAME);

		// �p�����[�^���Z�b�g
		prevProgressDistance = 0.0f;
	}

	void PlayerAttackState::UpdateBehavior()
	{
		// �U�����ɑO�i����
		ForwardProgressMove();

		// �U�������X�V���邩�m�F
		UpdateReAttack();

		// �R���r�l�[�V�����U���̓���
		UpdateCombInput();
	}

	void PlayerAttackState::OnEndBehavior()
	{
		ClearVelocity(false);
	}

	void PlayerAttackState::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
	{
		// �A�j���[�V�����̑J�ڐ悪�ҋ@��ԂȂ�ҋ@�ɖ߂�
		if (_toAnimNodeName == IDLE_ANIM_NAME)
			ChangeState(PlayerState::Idle);
	}

	void PlayerAttackState::SetAttackTimes(u_int _attackTimes)
	{
		attackTimeCnt = _attackTimes;
		OnChangeAttackTimes();
	}

	void PlayerAttackState::UpdateAttackInfo()
	{
		assert(curAttackTime <= attackTimeCnt && "�U���񐔂��I���Ă��܂�");
		assert(curAttackTime <= static_cast<u_int>(attackInfos.size()) && "�U���񐔂��I���Ă��܂�");

		// ���݂̍U���񐔖ڂ̏��𑗂�
		GetPlayer().SetAttackInfo(attackInfos[curAttackTime - 1]);
	}

	void PlayerAttackState::UpdateReAttack()
	{
		if (!pIsReAttack) return;	// ���A�^�b�N�ϐ����擾�ł��Ă��Ȃ�
		if (!(*pIsReAttack)) return;	// ���A�^�b�N���Ȃ��Ȃ珈�����Ȃ�

		// ���݂̍U���񐔂����֐i�߂�
		curAttackTime++;
		if (curAttackTime > attackTimeCnt)
		{
			HASHI_DEBUG_LOG("�U���񐔂����߂�ꂽ�񐔂𒴂��܂���");
			curAttackTime = attackTimeCnt;
		}

		UpdateAttackInfo();	// �U�������X�V
		GetAnimation()->SetBool(REATTACK_PARAMNAME, false);	// ���A�^�b�N�t���O���~�낷
	}

	void PlayerAttackState::UpdateCombInput()
	{
		if (!pActionController->GetCanInput()) return;	// ���͎󂯕t���Ă��Ȃ��Ȃ�

		if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
			pActionController->SetReserveState(static_cast<int>(nextCombAtkState));
	}

	void PlayerAttackState::CalcProgressDis(const DirectX::SimpleMath::Vector3& _atkEnemyPos)
	{
		if (!isMoveForward) return;

		// �ő勗���𒴂��Ȃ��ɋ��������߂�
		DXSimp::Vector3 distance = _atkEnemyPos - GetMyTransform().GetPosition();
		distance.y = 0.0f;
		curAtkProgressDis = distance.Length();
		if (curAtkProgressDis > atkMaxDistance)
			curAtkProgressDis = atkMaxDistance;
	}

	void PlayerAttackState::OnChangeAttackTimes()
	{
		// �U�������X�V
		attackInfos.resize(attackTimeCnt);
	}

	PlayerAttackInformation* PlayerAttackState::GetPlayerAtkInfo(u_int _atkIdx)
	{
		// �͈͊O�Ȃ�
		if (static_cast<u_int>(attackInfos.size()) <= _atkIdx)
		{
			HASHI_DEBUG_LOG("�G���[�F�U�����͈̔͊O���擾���Ă��܂�");
			return nullptr;
		}
			
		return &attackInfos[_atkIdx];
	}

	u_int PlayerAttackState::GetAttackTimes() const
	{
		return attackTimeCnt;
	}

	void PlayerAttackState::ImGuiDebug()
	{
		PlayerGroundState::ImGuiDebug();

		ImGuiCombAttack();

		// �U�����
		ImGui::Text("AtkInfo");
		int imInt = static_cast<int>(attackTimeCnt);	// �񐔎w��
		if (ImGui::DragInt("attackTime", &imInt, 1, 1, 100))
			SetAttackTimes(attackTimeCnt);

		ImGuiAttackInfo();

		// �O�i�J�[�u
		ImGui::Text(std::to_string(curAtkProgressDis).c_str());
		ImGui::Checkbox("MoveFwd", &isMoveForward);
		if (isMoveForward)
		{
			ImGui::DragFloat("AtkMoveSpeed", &atkMaxDistance, 0.1f);
			progressDistanceCurve.ImGuiCall();
		}
	}

	void PlayerAttackState::ImGuiAttackInfo()
	{
		if (!ImGuiMethod::TreeNode("Attack")) return;

		for (u_int a_i = 0; a_i < attackTimeCnt; a_i++)
		{
			std::string caption = "Step:" + std::to_string(a_i);
			if(!ImGuiMethod::TreeNode(caption)) continue;
			attackInfos[a_i].ImGuiCall();
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	void PlayerAttackState::InitParameter()
	{
		// ���A�^�b�N�ϐ�������Ă��Ȃ��Ȃ�
		if (!pIsReAttack)
			pIsReAttack = GetAnimation()->GetParameterPointer<bool>(REATTACK_PARAMNAME);

		curAttackTime = 1;	// 1�ڂ̍U����񂩂�
	}

	void PlayerAttackState::LookAtEnemyInstant(DirectX::SimpleMath::Vector3 _atkEnemyPos)
	{
		// �G�̕���������
		Transform& trans = GetMyTransform();
		_atkEnemyPos.y = trans.GetPosition().y;	// y����]�݂̂���

		trans.LookAt(_atkEnemyPos);
	}

	void PlayerAttackState::ForwardProgressMove()
	{
		using namespace DirectX::SimpleMath;

		float deltaTime = DeltaTime();
		if (deltaTime < Mathf::epsilon) return;

		if (!isMoveForward) return;

		CP_Animation* pAnimation = pActionController->GetAnimation();
		if (!pAnimation) return;

		// �J�[�u����i�ޗ����擾
		float animPlayRatio = pAnimation->GetCurrentPlayRatio();
		float curveVal = progressDistanceCurve.GetValue(animPlayRatio);
		float curDis = curveVal * curAtkProgressDis;
		float curSpeed = (curDis - prevProgressDistance) / deltaTime;

		Transform& transform = GetPlayer().GetTransform();

		// ���W�ɔ��f
		GetRB().SetVelocity(transform.Forward() * curSpeed);

		prevProgressDistance = curDis;
	}

	void PlayerAttackState::ImGuiCombAttack()
	{
#ifdef EDIT
		// �R���|�{�b�N�X�ŕύX
		std::string curStateStr = std::string(magic_enum::enum_name(nextCombAtkState));
		bool isChange = ImGuiMethod::ComboBox("NextCombAtk", curStateStr, combAtkState);

		if (isChange)
		{
			// �����񂩂�񋓌^
			auto changeState = magic_enum::enum_cast<PlayerState>(curStateStr);
			if (changeState.has_value())
			{
				nextCombAtkState = changeState.value();
				return;
			}
		}
#endif EDIT
	}

	nlohmann::json PlayerAttackState::Save()
	{
		auto data = PlayerGroundState::Save();

		data["nextCombAttack"] = nextCombAtkState;
		data["maxDistance"] = atkMaxDistance;
		data["attackCnt"] = attackTimeCnt;
		data["isMoveFwd"] = isMoveForward;
		data["forwardCurve"] = progressDistanceCurve.Save();

		// �U�������Z�[�u
		auto& attackInfoDatas = data["attackInfos"];
		for (u_int i = 0; i < attackTimeCnt; i++)
		{
			attackInfoDatas.push_back(attackInfos[i].Save());
		}

		return data;
	}

	void PlayerAttackState::Load(const nlohmann::json& _data)
	{
		using namespace HashiTaku;
		PlayerGroundState::Load(_data);

		LoadJsonEnum<PlayerState>("nextCombAttack", nextCombAtkState, _data);
		LoadJsonFloat("maxDistance", atkMaxDistance, _data);
		LoadJsonBoolean("isMoveFwd", isMoveForward, _data);

		// �U����
		if (LoadJsonUnsigned("attackCnt", attackTimeCnt, _data))
			SetAttackTimes(attackTimeCnt);

		// �U���������[�h
		nlohmann::json attackDatas;
		if (LoadJsonDataArray("attackInfos", attackDatas, _data))
		{
			u_int arrayIdx = 0;

			for (auto& atkInfoData : attackDatas)
			{
				attackInfos[arrayIdx].Load(atkInfoData);
				arrayIdx++;
			}
		}

		nlohmann::json loadData;
		if (LoadJsonData("forwardCurve", loadData, _data))
		{
			progressDistanceCurve.Load(loadData);
		}
	}
}