#include "pch.h"
#include "BossCombAttack.h"
#include "BossActionController.h"

BossCombAttack::BossCombAttack() : 
	pIsReAttack(nullptr), attackTimeCnt(1), curAttackTime(1)
{
}

void BossCombAttack::OnStartBehavior()
{
	BossAttackState::OnStartBehavior();

	// ���A�^�b�N�ϐ�������Ă��Ȃ��Ȃ�
	if (!pIsReAttack)
		pIsReAttack = GetAnimation()->GetParameterPointer<bool>(REATTACK_PARAMNAME);

	// ������
	curAttackTime = 1;	// 1�i�ڂ������
}

void BossCombAttack::UpdateBehavior()
{
	BossAttackState::UpdateBehavior();

	UpdateReAttack();
}

nlohmann::json BossCombAttack::Save()
{
	auto data = BossAttackState::Save();

	data["atkTimeCnt"] = attackTimeCnt;

	return data;
}

void BossCombAttack::Load(const nlohmann::json& _data)
{
	BossAttackState::Load(_data);

	HashiTaku::LoadJsonUnsigned("atkTimeCnt", attackTimeCnt, _data);
}

void BossCombAttack::UpdateReAttack()
{
	// ���A�^�b�N�̃^�C�~���O�łȂ��Ȃ�
	if (!(*pIsReAttack)) return;

	// ���A�^�b�N�t���O���~�낷
	GetAnimation()->SetBool(REATTACK_PARAMNAME, false);

	HASHI_DEBUG_LOG("a");

	// ���U���񐔂𒴂���Ȃ�
	if(curAttackTime + 1 > attackTimeCnt)	
	{
		HASHI_DEBUG_LOG("�G���[�F���U���񐔂𒴂��Ă��܂�");
		return;
	}

	// ���A�^�b�N�X�V
	curAttackTime++;

	assert(static_cast<u_int>(attackInfos.size()) >= curAttackTime && 
		"�U����񂪍U���񐔈ȉ��ł�");

	// �Ή������U�������Z�b�g����
	pActionController->SetAttackInfo(attackInfos[curAttackTime - 1]);
}

void BossCombAttack::SetAttackTimeCnt(u_int _attackTime)
{
#ifdef EDIT
	if (_attackTime == 0)
	{
		HASHI_DEBUG_LOG("1�ȏ��ݒ肵�Ă�������");
		return;
	}
#endif // EDIT

	attackTimeCnt = _attackTime;
	attackInfos.resize(_attackTime);	// �U���������킹��
}

void BossCombAttack::ImGuiDebug()
{
	BossAttackState::ImGuiDebug();

	// �U����
	int imInt = static_cast<u_int>(attackTimeCnt);
	if (ImGui::DragInt("attackCnt", &imInt, 1, 1, 100))
	{
		SetAttackTimeCnt(imInt);
	}
}
