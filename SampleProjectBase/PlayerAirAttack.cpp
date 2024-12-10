#include "pch.h"
#include "PlayerAirAttack.h"
#include "CP_Player.h"

namespace DXSimp = DirectX::SimpleMath;

// �u���Ɍ���̂𔽉f����G�Ƃ̋���
constexpr float INSTANTLOOK_DISTANCE(8.0f);

PlayerAirAttack::PlayerAirAttack() :
	nextCombAtkState(PlayerState::None),
	attackTimeCnt(1)
{
	attackInfos.resize(1);	// �U�������Œ�1�쐬���Ă���
	HashiTaku::AttackInformation atkInfo;
	attackInfos.push_back(atkInfo);
}

void PlayerAirAttack::OnStartBehavior()
{
	// �͈͓��Ȃ�G������
	LookAtEnemy();

	// �U�������X�V
	UpdateAttackInfo();

	// �U���t���O�𗧂Ă�
	pActionController->SetAnimationTrigger(ATTACKTRIGGER_PARAMNAME);

	// ���ɗ����Ȃ�����
	CastAirController().SetIsDownForce(false);
	ClearVelocity(true);
}

void PlayerAirAttack::UpdateBehavior()
{
	// �R���r�l�[�V�����U���̓���
	UpdateCombInput();

	// y�����x����0��
	ClearVelocityY();
}

void PlayerAirAttack::OnEndBehavior()
{
	// ������悤�ɂ���
	CastAirController().SetIsDownForce(true);
}

void PlayerAirAttack::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	if (_toAnimNodeName == FALLLOOP_ANIM_NAME)
		ChangeState(PlayerState::Move);
}

void PlayerAirAttack::UpdateCombInput()
{
	if (!pActionController->GetCanInput()) return;	// ���͎󂯕t���Ă��Ȃ��Ȃ�

	// �\���ԂɃZ�b�g
	if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
		 pActionController->SetReserveState(
			 static_cast<int>(nextCombAtkState), 
			 PlayerActionController_Base::CancelType::Attack
		 );
}

void PlayerAirAttack::LookAtEnemy()
{
	Transform& playerTrans = GetMyTransform();

	// �G���W�Ƃ̃x�N�g�����擾
	DXSimp::Vector3 atkPos = GetAtkEnemyPos();
	DXSimp::Vector3 vecToEnemy = atkPos - playerTrans.GetPosition();

	// �����͈͓��łȂ��Ȃ珈�����Ȃ�
	if (vecToEnemy.Length() > INSTANTLOOK_DISTANCE)
		return;

	// �G�̕���������(y����]�݂̂���)
	atkPos.y = playerTrans.GetPosition().y;	
	playerTrans.LookAt(atkPos);
}

void PlayerAirAttack::UpdateAttackInfo()
{
	// ���݂̍U���񐔖ڂ̏��𑗂�
	GetPlayer().SetAttackInfo(attackInfos[0]);
}

void PlayerAirAttack::ClearVelocityY()
{
	CP_RigidBody& rb = GetRB();
	rb.ClearVelocity();
}
