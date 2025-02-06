#include "pch.h"
#include "PlayerAirAttack.h"
#include "PlayerAirActionController.h"
#include "CP_Player.h"

namespace HashiTaku
{
	// �u���Ɍ���̂𔽉f����G�Ƃ̋���
	constexpr float INSTANTLOOK_DISTANCE(8.0f);

	PlayerAirAttack::PlayerAirAttack() :
		nextCombAtkState(PlayerState::None),
		attackTimeCnt(1)
	{
	}

	void PlayerAirAttack::InitState()
	{
		attackInfos.resize(1);	// �U�������Œ�1�쐬���Ă���
		attackInfos[0] = CreateAttackInfo();
	}

	void PlayerAirAttack::OnStartBehavior()
	{
		// �͈͓��Ȃ�G������
		LookAtEnemy();

		// �U�������X�V
		UpdateAttackInfo();

		// �U���t���O�𗧂Ă�
		GetActionController().SetAnimationTrigger(ATTACKTRIGGER_PARAMNAME);

		// ���ɗ����Ȃ�����
		GetAirController().SetIsDownForce(false);
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
		GetAirController().SetIsDownForce(true);
	}

	void PlayerAirAttack::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
	{
		if (_toAnimNodeName == FALLLOOP_ANIM_NAME)
			ChangeState(PlayerState::Move);
	}

	std::unique_ptr<PlayerAttackInformation> PlayerAirAttack::CreateAttackInfo()
	{
		return std::make_unique<PlayerAttackInformation>(&GetAirController().GetPlayer());
	}

	void PlayerAirAttack::UpdateCombInput()
	{
		PlayerAirActionController& actCon = GetAirController();

		if (!actCon.GetCanInput()) return;	// ���͎󂯕t���Ă��Ȃ��Ȃ�

		// �\���ԂɃZ�b�g
		if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
			actCon.SetReserveState(static_cast<int>(nextCombAtkState));
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
		GetPlayer().SetAttackInfo(*attackInfos[0]);
	}

	void PlayerAirAttack::ClearVelocityY()
	{
		IActionController& actCon = GetActionController();

		// y���x����0�ɂ���
		DXSimp::Vector3 curVelocity;
		actCon.GetVelocity(curVelocity);
		curVelocity.y = 0.0f;

		GetActionController().SetVelocity(curVelocity);
	}
}