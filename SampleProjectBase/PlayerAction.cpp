#include "pch.h"
#include "PlayerAction.h"
#include "InSceneSystemManager.h"

// �A�N�V�����ł��鍶�X�e�B�b�N�̓��͗�
constexpr float CAN_ACTION_STICKINPUT(0.7f);

// �A�j���[�V�����R���g���[�����̃v���C���[��
constexpr  auto STATEANIM_PARAMNAME{ "state" };
// �^�[�Q�b�g����\���A�j���[�V�����p�����[�^
constexpr auto TARGET_PARAMNAME{ "targeting" };
// �L�����Z���ł��邩��\���A�j���[�V�����p�����[�^
constexpr auto CANCEL_PARAMNAME{ "canCancel" };
// ��s���͂ł��邩��\���A�j���[�V�����p�����[�^
constexpr auto INPUT_PARAMNAME{ "canInput" };
// �U���ł��邩��\���A�j���[�V�����p�����[�^
constexpr auto CANATK_PARAMNAME{ "canAttack" };
// �ړ��ł��邩��\���A�j���[�V�����p�����[�^
constexpr auto CANMOVE_PARAMNAME{ "canMove" };

PlayerAction::PlayerAction()
{
	InSceneSystemManager* pInsceneSysytem = InSceneSystemManager::GetInstance();
	pInput = &pInsceneSysytem->GetInput();

	pGroundContoroller = std::make_unique<PlayerActionController>();

	pAirContoroller = std::make_unique<PlayerActionController>();
}

PlayerAction::~PlayerAction()
{
	// �^�[�Q�b�g���鑤�ɍ폜����悤�ɒʒm
	if (pTargetObject)
		pTargetObject->RemoveTargeter(*this);
}

void PlayerAction::Init(CP_Animation* _pAnimation, CP_RigidBody* _pRigidBody)
{
	InSceneSystemManager* pInsceneSysytem = InSceneSystemManager::GetInstance();
	pBattleManager = CP_BattleManager::GetInstance();
	pAnimation = _pAnimation;
	pCamera = &pInsceneSysytem->GetMainCamera();
	if (pCamera)
	{
		pTargetCamera = pCamera->GetGameObject().GetComponent<CP_CameraMove>();
	}
	// �A�j���[�V�����p�����[�^�̃A�h���X���擾
	pIsCanCancel = pAnimation->GetParameterPointer<bool>(CANCEL_PARAMNAME);
	// ��s���͂̃A�h���X���擾
	pIsCanInput = pAnimation->GetParameterPointer<bool>(INPUT_PARAMNAME);
	// �U���\�A�h���X���擾
	pIsCanAttack = pAnimation->GetParameterPointer<bool>(CANATK_PARAMNAME);
	// �ړ��\�A�h���X���擾
	pIsCanMove = pAnimation->GetParameterPointer<bool>(CANMOVE_PARAMNAME);

	pGroundContoroller->Init(pAnimation,_pRigidBody);

	pAirContoroller->Init(pAnimation, _pRigidBody);
}

void PlayerAction::Update()
{
}

CP_Camera& PlayerAction::GetCamera()
{
	return *pCamera;
}

const DirectX::SimpleMath::Vector2& PlayerAction::GetInputLeftStick() const
{
	return  pInput->GetValue(GameInput::ValueType::Player_Move);
}

bool PlayerAction::IsInputVector(InputVector _checkVector)
{
	// �{�^������
	if (!pInput->GetButtonDown(GameInput::ButtonType::Player_Rolling)) return false;

	// ���[�����O�ł��鍶�X�e�B�b�N�̓��͂��ł��Ă��Ȃ�
	if (std::min(GetInputLeftStick().Length(), 1.0f) < CAN_ACTION_STICKINPUT) return false;

	return true;
}

bool PlayerAction::GetIsTargeting() const
{
	return isTargeting;
}

bool PlayerAction::GetCanInput() const
{
	assert(pIsCanInput);
	return *pIsCanInput;
}

bool PlayerAction::GetIsCanCancel() const
{
	assert(pIsCanCancel);
	return *pIsCanCancel;
}

bool PlayerAction::GetCanAttack() const
{
	assert(pIsCanAttack);
	return *pIsCanAttack;
}

bool PlayerAction::GetCanMove() const
{
	assert(pIsCanMove);
	return *pIsCanMove; return false;
}

nlohmann::json PlayerAction::Save()
{
	nlohmann::json data;

	data["ground"] = pGroundContoroller->Save();
	data["air"] = pAirContoroller->Save();

	return data;
}

void PlayerAction::Load(const nlohmann::json& _data)
{
}

void PlayerAction::OnBeginTargeting()
{
	if (!pBattleManager) return;

	// �G���X�g���擾����
	auto& enemyList = pBattleManager->GetEnemyList();
	u_int enemyCnt = static_cast<u_int>(enemyList.size());

	if (enemyCnt == 0) return;

	// ���G�������̂ł�悤�ɂ���Ȃ�ǂ̃^�[�Q�b�g�ɂ��邩�̏����������ɏ���
	// ����̓{�X1�̂Ȃ̂ł���������
	ITargetAccepter* pTargetEnemy = *enemyList.begin();

	SetTargetObject(*pTargetEnemy);

	if (pTargetCamera)	// �J�����ɂ��`����
		pTargetCamera->SetTargetObject(*pTargetEnemy);
}

void PlayerAction::OnEndTargeting()
{
	if (!pTargetObject) return;

	if (pTargetCamera)	// �J�����ɂ��`����
	{
		pTargetCamera->RemoveNotify(*pTargetObject);
		pTargetObject->RemoveTargeter(*pTargetCamera);
	}

	// �^�[�Q�b�g�ŎQ�Ƃ���̂���߂�
	pTargetObject->RemoveTargeter(*this);
	pTargetObject = nullptr;
}

void PlayerAction::UpdateTargeting()
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
