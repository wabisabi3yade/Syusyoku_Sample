#include "pch.h"
#include "PlayerActState_Base.h"
#include "PlayerActionController_Base.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	GameInput* PlayerActState_Base::pPlayerInput = nullptr;

	// �A�N�V�����ł��鍶�X�e�B�b�N�̓��͗�
	constexpr float CAN_ACTION_STICKINPUT(0.7f);
	// �X�e�B�b�N�̕����̓��ςł���ȏ��v���Ă��Ȃ��Ȃ甽�����Ȃ�
	constexpr float INPUT_VECTOR_DOT(0.6f);
	// �ړ��ŃL�����Z�����ł���X�e�B�b�N�̓��͗�
	constexpr float CAN_MOVECANCEL_INPUT(0.3f);
	// �L�����Z�����͂ŗ\�񂵂���Ԃ̗L���������ԁi������Ɨ\�񂵂���Ԃ̓��Z�b�g�����j
	constexpr float CANCEL_RESERVE_VALIED_TIME(0.3f);

	namespace DXSimp = DXSimp;

	PlayerActState_Base::PlayerActState_Base() :
		cancelType(CancelType::None),
		statePriority(-1),
		targetLookRotateSpeed(40.0f),
		isTargetLookAtEnemy(false)
	{
		pPlayerInput = &InSceneSystemManager::GetInstance()->GetInput();
	}

	void PlayerActState_Base::Init(PlayerActionController_Base& _actController, 
		CancelType _cancelType,
		int _priority)
	{
		CharacterActState_Base::Init(_actController);
		cancelType = _cancelType;
		statePriority = _priority;

		// �X�e�[�g�̏�����
		InitState();
	}

	void PlayerActState_Base::Update()
	{
		CharacterActState_Base::Update();

		// �^�[�Q�b�g�̕���������
		UpdateTargetLook();
	}

	PlayerActState_Base::CancelType PlayerActState_Base::GetCancelType() const
	{
		return cancelType;
	}

	int PlayerActState_Base::GetPriority() const
	{
		return statePriority;
	}

	json PlayerActState_Base::Save()
	{
		return json();
	}

	void PlayerActState_Base::Load(const json& _data)
	{
	}

	void PlayerActState_Base::ClearVelocity(bool _applyY)
	{
		DXSimp::Vector3 set;
		IActionController& actCon = GetActionController();
		if (!_applyY) // y�����f���������Ȃ��Ȃ�
		{
			set.y = actCon.GetVelocity().y;
		}
		actCon.SetVelocity(set);
	}

	void PlayerActState_Base::SetTargetAtEnemy(bool _isLook)
	{
		isTargetLookAtEnemy = _isLook;
	}

	void PlayerActState_Base::SetInvicible(bool _isInvicible)
	{
		GetPlayer().SetIsInvicible(_isInvicible);
	}

	CP_Animation* PlayerActState_Base::GetAnimation()
	{
		return GetActionController().GetAnimation();
	}

	const ITargetAccepter* PlayerActState_Base::GetTargetAccepter()
	{
		return GetPlayerActionController().GetTargetAccepter();
	}

	PlayerActState_Base* PlayerActState_Base::GetPlayerState(int _getStateId)
	{
		return GetPlayerActionController().GetPlayerState(_getStateId);
	}

	CP_Player& PlayerActState_Base::GetPlayer()
	{
		return GetPlayerActionController().GetPlayer();
	}

	float PlayerActState_Base::GetDeltaSpeed()
	{
		return GetActionController().GetCharacter().GetDeltaSpeed();
	}

	DXSimp::Vector2 PlayerActState_Base::GetInputLeftStick() const
	{
		return pPlayerInput->GetValue(GameInput::ValueType::Player_Move);
	}

	DXSimp::Vector2 PlayerActState_Base::GetInputLeftStickFromCam()
	{
		// ���͒l���擾
		DXSimp::Vector2 inputVec = GetInputLeftStick();

		// �J�������猩�����͂Ƃ���
		const Transform& camTrans = GetPlayerActionController().GetCamera().GetTransform();
		DXSimp::Vector3 inputVecByCam = inputVec.x * camTrans.Right() +
			inputVec.y * camTrans.Forward();
		inputVec = { inputVecByCam.x, inputVecByCam.z };
		inputVec.Normalize();

		return inputVec;
	}

	DXSimp::Vector3 PlayerActState_Base::GetAtkEnemyPos()
	{
		PlayerActionController_Base& actCon = GetPlayerActionController();

		// �^�[�Q�b�g���Ă���Ȃ�
		if (actCon.GetIsTargeting())
		{
			auto* pAccepter = actCon .GetTargetAccepter();
			if (pAccepter)
				return pAccepter->GetWorldPos();
		}

		// �߂��̓G��T��
		CP_BattleManager* pBattle = CP_BattleManager::GetInstance();
		if (!pBattle) return DXSimp::Vector3::Zero;

		// �G���X�g���擾����
		const auto& enemyList = pBattle->GetEnemyList();

		// �G�����Ȃ��Ȃ�
		if (static_cast<u_int>(enemyList.size()) == 0) return DXSimp::Vector3::Zero;

		// �G�̍U�����W
		DXSimp::Vector3 atkEnemyPos;
		const ITargetAccepter* pAtkEnemy = (*enemyList.begin());

		// �{���͈�ԋ߂��̓G���擾����
		atkEnemyPos = pAtkEnemy->GetWorldPos();

		return atkEnemyPos;
	}

	bool PlayerActState_Base::IsInputVector(InputVector _checkVector)
	{
		// ���͒l���擾
		DXSimp::Vector2 inputVec = GetInputLeftStick();
		if (inputVec.Length() < CAN_ACTION_STICKINPUT) return false;	// �X�����\���łȂ��Ȃ�

		PlayerActionController_Base& actCon = GetPlayerActionController();

		// �J�������猩�����͂Ƃ���
		const Transform& camTrans = actCon.GetCamera().GetTransform();
		DXSimp::Vector3 inputVecByCam = inputVec.x * camTrans.Right() +
			inputVec.y * camTrans.Forward();
		inputVec = { inputVecByCam.x, inputVecByCam.z };
		inputVec.Normalize();

		// �������擾
		DXSimp::Vector3 baseVec;

		// �^�[�Q�b�g���œG������Ȃ�@�G�̕����x�N�g������x�N�g����
		if (actCon.GetIsTargeting() &&
			GetTargetAccepter())
		{
			DXSimp::Vector3 targetObjVec =
				GetTargetAccepter()->GetWorldPos() -
				GetMyTransform().GetPosition();

			targetObjVec.Normalize(baseVec);
		}
		else // �Ⴄ�Ȃ�@�v���C���[�̑O�x�N�g��
			baseVec = GetPlayer().GetTransform().Forward();

		DXSimp::Vector2 baseVecXZ = { baseVec.x, baseVec.z };
		baseVecXZ.Normalize();

		// �X�e�B�b�N�̕�������v���Ă��邩����
		float dot = inputVec.Dot(baseVecXZ);

		return dot > INPUT_VECTOR_DOT ? true : false;
	}

	bool PlayerActState_Base::IsRollingInput() const
	{
		// �{�^������
		if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Rolling)) return false;

		// ���[�����O�ł��鍶�X�e�B�b�N�̓��͂��ł��Ă��Ȃ�
		if (std::min(GetInputLeftStick().Length(), 1.0f) < CAN_ACTION_STICKINPUT) return false;

		return true;
	}

	bool PlayerActState_Base::IsSpecialAtkInput(InputVector _inputVecter)
	{
		// �{�^������
		if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack)) return false;

		// �^�[�Q�b�g���Ă��Ȃ��Ȃ�
		if (!GetPlayerActionController().GetIsTargeting()) return false;

		if (!IsInputVector(_inputVecter)) return false;

		return true;
	}

	void PlayerActState_Base::UpdateTargetLook()
	{
		// �^�[�Q�b�g�̕������Ȃ��Ȃ�
		if (!isTargetLookAtEnemy) return;

		PlayerActionController_Base& actCon = GetPlayerActionController();
		const ITargetAccepter* pTargetObj = actCon.GetTargetAccepter();
		if (!pTargetObj) return;	// �^�[�Q�b�g�����Ȃ��Ȃ�

		Transform& transform = actCon.GetMyTransform();

		// �^�[�Q�b�g�ւ̃x�N�g�������߂�
		const DXSimp::Vector3& playerPos = transform.GetPosition();
		const DXSimp::Vector3& targetPos = pTargetObj->GetWorldPos();
		DXSimp::Vector3 vector = targetPos - playerPos;
		vector.y = 0.0f; vector.Normalize();

		// ��]������
		DXSimp::Quaternion targetRot = Quat::RotateToVector(vector);
		DXSimp::Quaternion myRot = transform.GetRotation();
		myRot = DXSimp::Quaternion::Slerp(myRot, 
			targetRot, 
			targetLookRotateSpeed * DeltaTime());

		transform.SetRotation(myRot);
	}
	PlayerActionController_Base& PlayerActState_Base::GetPlayerActionController()
	{
		return GetDeliverActionController<PlayerActionController_Base>();
	}
}