#include "pch.h"
#include "PlayerAction.h"
#include "InSceneSystemManager.h"

// �A�N�V�����ł��鍶�X�e�B�b�N�̓��͗�
constexpr float CAN_ACTION_STICKINPUT(0.7f);

// �ړ����x
//constexpr auto SPEEDRATIO_PARAMNAME{ "speed" };
// �^�[�Q�b�g����\���A�j���[�V�����p�����[�^
constexpr auto TARGET_PARAMNAME{ "targeting" };
// �ڒn���Ă��邩��\���A�j���[�V�����p�����[�^
constexpr auto GROUND_PARAMNAME{ "isGround" };
// �L�����Z���ł��邩��\���A�j���[�V�����p�����[�^
constexpr auto CANCEL_PARAMNAME{ "canCancel" };
// ��s���͂ł��邩��\���A�j���[�V�����p�����[�^
constexpr auto INPUT_PARAMNAME{ "canInput" };
// �U���ł��邩��\���A�j���[�V�����p�����[�^
constexpr auto CANATK_PARAMNAME{ "canAttack" };
// �ړ��ł��邩��\���A�j���[�V�����p�����[�^
constexpr auto CANMOVE_PARAMNAME{ "canMove" };

PlayerAction::PlayerAction(CP_Player& _player) :
	pGroundChecker(nullptr),
	pBattleManager(nullptr),
	pTargetAccepter(nullptr),
	pTargetCamera(nullptr),
	pCamera(nullptr),
	pAnimation(nullptr),
	pIsCanInput(nullptr),
	pIsCanAttack(nullptr),
	pIsCanCancel(nullptr),
	pIsCanMove(nullptr),
	isGround(true),
	prevIsGround(true),
	reserveAirTransition(GroundToAir::ToAirMove),
	reserveGroundTransition(AirToGround::AirToLanding),
	isTargeting(false),
	prevIsTargeting(false)
{
	InSceneSystemManager* pInsceneSysytem = InSceneSystemManager::GetInstance();
	pPlayer = &_player;
	pInput = &pInsceneSysytem->GetInput();
	pGroundController = std::make_unique<PlayerGroundActionController>(*this, _player);
	pAirController = std::make_unique<PlayerAirActionController>(*this, _player);
}

PlayerAction::~PlayerAction()
{
	// �^�[�Q�b�g���鑤�ɍ폜����悤�ɒʒm
	if (pTargetAccepter)
		pTargetAccepter->RemoveTargeter(*this);
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
	if (pAnimation)
	{
		// �A�j���[�V�����p�����[�^�̃A�h���X���擾
		pIsCanCancel = pAnimation->GetParameterPointer<bool>(CANCEL_PARAMNAME);
		// ��s���͂̃A�h���X���擾
		pIsCanInput = pAnimation->GetParameterPointer<bool>(INPUT_PARAMNAME);
		// �U���\�A�h���X���擾
		pIsCanAttack = pAnimation->GetParameterPointer<bool>(CANATK_PARAMNAME);
		// �ړ��\�A�h���X���擾
		pIsCanMove = pAnimation->GetParameterPointer<bool>(CANMOVE_PARAMNAME);
	}

	// �ڒn�`�F�b�J�[���擾
	FindGroundChecker();

	// �A�N�V����������
	pGroundController->Init(pAnimation, _pRigidBody);
	pAirController->Init(pAnimation, _pRigidBody);

	pCurrentController = pGroundController.get();
	currentPlace = ActionPlace::Ground;
}

void PlayerAction::Update()
{
	pCurrentController->UpdateCall();

	UpdateTargeting();

	UpdateGround();
}

void PlayerAction::OnChangePlace()
{
}

CP_Camera& PlayerAction::GetCamera()
{
	return *pCamera;
}

const ITargetAccepter* PlayerAction::GetTargetAccepter() const
{
	return pTargetAccepter;
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

bool PlayerAction::GetCanCancel() const
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

bool PlayerAction::GetIsGround() const
{
	return isGround;
}

void PlayerAction::SetTargetObject(ITargetAccepter& _targetObject)
{
	pTargetAccepter = &_targetObject;
	pTargetAccepter->SetTargeter(*this);
}

void PlayerAction::RemoveNotify(const ITargetAccepter& _deathTargetObj)
{
	// ���񂾃^�[�Q�b�g�悪���݌��Ă���I�u�W�F�N�g��������Q�Ƃ�����
	if (!pTargetAccepter) return;
	if (pTargetAccepter != &_deathTargetObj) return;

	pTargetAccepter = nullptr;
}

void PlayerAction::OnDamage(const HashiTaku::AttackInformation& _atkInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos,
	bool* _pAcceptDamage)
{
	pCurrentController->OnDamage(_atkInfo,
		_attackerPos,
		_pAcceptDamage);
}

void PlayerAction::OnGroundEnter()
{
	// �ڒn�t���O�ƃA�j���[�V�����̕���true����
	isGround = true;
	if (pAnimation)
	{
		pAnimation->SetBool(GROUND_PARAMNAME, isGround);
	}
	HASHI_DEBUG_LOG("���n");
}

void PlayerAction::OnGroundExit()
{
	// �ڒn�t���O�ƃA�j���[�V�����̕���false����
	isGround = false;
	if (pAnimation)
	{
		pAnimation->SetBool(GROUND_PARAMNAME, isGround);
	}
	HASHI_DEBUG_LOG("����");
}

nlohmann::json PlayerAction::Save()
{
	nlohmann::json data;

	data["groundCheckName"] = groundCheckerName;

	data["ground"] = pGroundController->Save();
	data["air"] = pAirController->Save();

	return data;
}

void PlayerAction::Load(const nlohmann::json& _data)
{
	HashiTaku::LoadJsonString("groundCheckName", groundCheckerName, _data);

	nlohmann::json loadData;
	if (HashiTaku::LoadJsonData("ground", loadData, _data))
	{
		pGroundController->Load(loadData);
	}
	if (HashiTaku::LoadJsonData("air", loadData, _data))
	{
		pAirController->Load(loadData);
	}
}

void PlayerAction::FindGroundChecker()
{
	if (groundCheckerName.empty()) return;

	// ���O����I�u�W�F�N�g���擾
	auto& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();
	GameObject* go = sceneObjs.GetSceneObject(groundCheckerName);
	if (!go) return;

	// �ڒn�`�F�b�J�[�R���|�[�l���g���擾
	pGroundChecker = go->GetComponent<CP_GroundCheck>();
	if (!pGroundChecker)
	{
		HASHI_DEBUG_LOG("CP_GroundChecker���I�u�W�F�N�g�ɂ���܂���");
		return;
	}

	// �ڒn�`�F�b�J�[�ɃZ�b�g����
	pGroundChecker->SetGroundNotifyer(*this);
}

bool PlayerAction::GetCanUpdate()
{
	if (!pCamera) return false;

	return true;
}

void PlayerAction::ActionUpdate()
{
	pCurrentController->UpdateCall();
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
	if (!pTargetAccepter) return;

	if (pTargetCamera)	// �J�����ɂ��`����
	{
		pTargetCamera->RemoveNotify(*pTargetAccepter);
		pTargetAccepter->RemoveTargeter(*pTargetCamera);
	}

	// �^�[�Q�b�g�ŎQ�Ƃ���̂���߂�
	pTargetAccepter->RemoveTargeter(*this);
	pTargetAccepter = nullptr;
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

void PlayerAction::UpdateGround()
{
	// �O�t���[���Ɠ����Ȃ珈�����Ȃ�
	if (isGround == prevIsGround) return;

	// �ύX�O�̏I������
	pCurrentController->OnEnd();

	if (isGround) 	// �n��ֈڍs
		OnAirToGround();
	else	// �󒆂ֈڍs
		OnGroundToAir();

	prevIsGround = isGround;	// �X�V
}

void PlayerAction::OnGroundToAir()
{
	currentPlace = ActionPlace::Air;

	// ���݂̃R���g���[���[��ύX
	pCurrentController = pAirController.get();

	// �󒆂̃G���g���[��Ԃ���ݒ�
	switch (reserveAirTransition)
	{
	case PlayerAction::GroundToAir::ToAirMove:
		pAirController->ChangeAirState(PlayerAirActionController::AirState::Move);
		break;

	default:
		break;
	}

	// �f�t�H���g�ɖ߂�
	reserveAirTransition = GroundToAir::ToAirMove;
}

void PlayerAction::OnAirToGround()
{
	currentPlace = ActionPlace::Ground;

	// ���݂̃R���g���[���[��ύX
	pCurrentController = pGroundController.get();

	// �n��̃G���g���[��Ԃ���ݒ�
	switch (reserveGroundTransition)
	{
	case AirToGround::AirToLanding:
		pGroundController->
			ChangeGroundState(PlayerGroundActionController::GroundState::Idle, true);
		break;
	case AirToGround::AirToGroundKnock:
		pGroundController->
			ChangeGroundState(PlayerGroundActionController::GroundState::Move, true);
		break;
	default:
		break;
	}

	// �f�t�H���g�ɖ߂�
	reserveGroundTransition = AirToGround::AirToLanding;
}

void PlayerAction::ImGuiDebug()
{
	ImGuiMethod::Text("isGround", isGround);
	ImGuiMethod::Text("IsTargeting", isTargeting);
	ImGui::Text("Place:%s", std::string(magic_enum::enum_name(currentPlace)).c_str());

	if (ImGuiMethod::TreeNode("Ground"))
	{
		pGroundController->ImGuiCall();
		ImGui::TreePop();
	}

	if (ImGuiMethod::TreeNode("Air"))
	{
		pAirController->ImGuiCall();
		ImGui::TreePop();
	}
}

void PlayerAction::ImGuiGroundChecker()
{
#ifdef EDIT
	char input[IM_INPUT_BUF];
	std::strcpy(input, groundCheckerName.c_str());
	if (ImGui::InputText("GroundChecker", input, IM_INPUT_BUF))
	{
		groundCheckerName = input;
	}
#endif // EDIT
}
