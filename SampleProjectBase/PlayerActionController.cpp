#include "pch.h"
#include "PlayerActionController.h"
#include "CP_Animation.h"
#include "InSceneSystemManager.h"
#include "CP_BattleManager.h"

#include "PlayerIdleState.h"
#include "PlayerMoveState.h"
#include "PlayerTargetMove.h"
#include "PlayerRollingMove.h"
#include "PlayerAttackState.h"
#include "PlayerGroundAttack.h"
#include "PlayerRushAttack.h"

PlayerActionController::PlayerActionController(CP_Player& _player) :
	CharacterActionController(_player, "playerAction"), pBattleManager(nullptr),
	pIsCanCancel(nullptr), pTargetObject(nullptr), pIsCanCombAtk(nullptr), 
	isTargeting(false), prevIsTargeting(false)
{
	pInput = &InSceneSystemManager::GetInstance()->GetInput();

	// �s���N���X�𐶐�
	using enum PlayerActState_Base::PlayerState;
	CreateState<PlayerIdleState>(Idle);
	CreateState<PlayerMoveState>(Move);
	CreateState<PlayerTargetMove>(TargetMove);
	CreateState<PlayerRollingMove>(Rolling);
	CreateState<PlayerGroundAttack>(Attack11);
	CreateState<PlayerGroundAttack>(Attack12);
	CreateState<PlayerGroundAttack>(Attack13);
	CreateState<PlayerGroundAttack>(Attack14);
	CreateState<PlayerRushAttack>(SpecialAtkHi);

	// �f�t�H���g��Ԃ��Z�b�g
	SetDefaultNode(static_cast<int>(Idle));
}
void PlayerActionController::Init(CP_Animation* _animationController,
	CP_RigidBody* _pRigidBody)
{
	pBattleManager = CP_BattleManager::GetInstance();

	// �X�e�[�g�}�V�����ʊJ�n����
	CharacterActionController::Init(_animationController, _pRigidBody);

	// �A�j���[�V�����p�����[�^�̃A�h���X���擾
	pIsCanCancel = pAnimation->GetParameterPointer<bool>(CANCEL_PARAMNAME);

	// ��s���͂̃A�h���X���擾
	pIsCanInput = pAnimation->GetParameterPointer<bool>(INPUT_PARAMNAME);

	// �R���r�l�[�V�����U��k�\�A�h���X���擾
	pIsCanCombAtk = pAnimation->GetParameterPointer<bool>(COMBATK_PARAMNAME);
}

bool PlayerActionController::GetCanUpdate()
{
	if (!pRigidBody)
	{
		assert(!"rigidBody��ݒ肵�Ă�������");
		return false;
	}	

	return true;
}

void PlayerActionController::Update()
{
#ifdef EDIT
	if (!GetCanUpdate()) return;
#endif // EDIT

	pCurrentNode->Update();

	UpdateTargeting();
}

void PlayerActionController::OnBeginTargeting()
{
	if (!pBattleManager) return;

	// �G���X�g���擾����
	auto& enemyList = pBattleManager->GetEnemyList();
	u_int enemyCnt = static_cast<u_int>(enemyList.size());

	if (enemyCnt == 0) return;

	// ���G�������̂ł�悤�ɂ���Ȃ�ǂ̃^�[�Q�b�g�ɂ��邩�̏����������ɏ���
	// ����̓{�X1�̂Ȃ̂ł���������

	SetTargetObject(*(*enemyList.begin()));
}

void PlayerActionController::OnEndTargeting()
{
	// �^�[�Q�b�g�ŎQ�Ƃ���̂���߂�
	pTargetObject = nullptr;
}

void PlayerActionController::UpdateTargeting()
{
	// �{�^�������ꂽ��^�[�Q�b�g��
	isTargeting = pInput->GetButton(GameInput::ButtonType::Player_RockOn);

	// �A�j���[�V�����p�����[�^�ɂ�����
	pAnimation->SetBool(TARGET_PARAMNAME, isTargeting);

	if (isTargeting && !prevIsTargeting)	// �^�[�Q�b�g�����u��
		OnBeginTargeting();
	else if (!isTargeting && prevIsTargeting)	// �^�[�Q�b�g�����̏u��
		OnEndTargeting();

	// ���t���[���ׂ̈ɍX�V
	prevIsTargeting = isTargeting;
}

bool PlayerActionController::ChangeState(const PlayerActState_Base::PlayerState& _nextActionState)
{
	// �X�e�[�g�}�V���ŕύX
	bool isSuccess = StateMachine_Base::ChangeNode(static_cast<int>(_nextActionState));
	if (!isSuccess) return false;

	// �A�j���[�V��������state�ϐ����ύX
	pAnimation->SetInt(STATEANIM_PARAMNAME, static_cast<int>(_nextActionState));

	return true;
}

bool PlayerActionController::GetIsTargeting() const
{
	return isTargeting;
}

bool PlayerActionController::GetIsPrevTargeting() const
{
	return prevIsTargeting;
}

bool PlayerActionController::GetIsCanCancel() const
{
	return *pIsCanCancel;
}

bool PlayerActionController::GetCanInput() const
{
	return *pIsCanInput;
}

bool PlayerActionController::GetCanCombAtk() const
{
	return *pIsCanCombAtk;
}

CP_Player& PlayerActionController::GetPlayer()
{
	return static_cast<CP_Player&>(*pCharacter);
}

PlayerActState_Base& PlayerActionController::CastPlayerAct(HashiTaku::StateNode_Base& _stateNodeBase)
{
	return static_cast<PlayerActState_Base&>(_stateNodeBase);
}

void PlayerActionController::ImGuiDebug()
{
	ImGuiMethod::Text("isTargeting", isTargeting);

	CharacterActionController::ImGuiDebug();
}

PlayerActState_Base* PlayerActionController::GetCurrentAction()
{
	return static_cast<PlayerActState_Base*>(pCurrentNode);
}

std::string PlayerActionController::GetStateStr(int _stateId)
{
	PlayerActState_Base::PlayerState state =
		static_cast<PlayerActState_Base::PlayerState>(_stateId);

	return std::string(magic_enum::enum_name<PlayerActState_Base::PlayerState>(state));
}

int PlayerActionController::GetStateId(const std::string& _stateName)
{
	auto state = magic_enum::enum_cast<PlayerActState_Base::PlayerState>(_stateName);
	if (!state.has_value())
	{
		assert("����ɏ�Ԃ��擾�ł��܂���");
		return -1;
	}

	return static_cast<int>(state.value());
}

ITargetAccepter* PlayerActionController::GetTargetObject()
{
	return pTargetObject;
}

void PlayerActionController::SetTargetObject(ITargetAccepter& _targetObject)
{
	pTargetObject = &_targetObject;
}

void PlayerActionController::UpdateDeathNotify(const ITargetAccepter& _deathTargetObj)
{
	// ���񂾃^�[�Q�b�g�悪���݌��Ă���I�u�W�F�N�g��������Q�Ƃ�����
	if (!pTargetObject) return;
	if (pTargetObject != &_deathTargetObj) return;

	pTargetObject = nullptr;
}

