#include "pch.h"
#include "BossActionController.h"
#include "CP_Player.h"
#include "CP_Boss.h"
#include "CP_BattleManager.h"

#include "BossIdleState.h"
#include "BossGroundMove.h"
#include "BossDamageState.h"
#include "BossCombAttack.h"
#include "BossJumpAttack.h"


// ��Ԃ�\���A�j���[�V�����p�����[�^��
constexpr auto STATE_PARAM_NAME("state");
// �̂����肪�ł��邩�A�j���[�V�����p�����[�^��
constexpr auto CAN_KNOCK_PARAM_NAME("canKnock");

BossActionController::BossActionController(CP_Boss& _boss) :
	EnemyActionController(_boss, "bossActCon"), pPlayerObject(nullptr), pCanKnock(nullptr),
	defaultState(BossActState_Base::BossState::Idle)
{
	// �X�e�[�g�쐬
	using enum BossActState_Base::BossState;
	using enum ActDistance;
	CreateState<BossIdleState>(Idle, { ActDistance::Far });
	CreateState<BossGroundMove>(Walk, { ActDistance::Far });
	CreateState<BossGroundMove>(Run, { ActDistance::Far });

	CreateState<BossDamageState>(Damage_Small, {});

	CreateState<BossCombAttack>(CombAttack1, { ActDistance::Short });
	CreateState<BossJumpAttack>(JumpAttack, 
		{ ActDistance::Far,ActDistance::Short });

	// �f�t�H���g�m�[�h�ݒ�
	SetDefaultNode(static_cast<int>(defaultState));
}

void BossActionController::Init(CP_Animation* _pAnimation, CP_RigidBody* _pRigidBody)
{
	EnemyActionController::Init(_pAnimation, _pRigidBody);

	// �A�j���[�V�����p�����[�^����ϐ��̃|�C���^���炤
	GetAnimationParam();
}

void BossActionController::Update()
{
	if (!IsCanBossUpdate()) return;

	EnemyActionController::Update();
}

bool BossActionController::ChangeState(BossActState_Base::BossState _nextState, bool _isForce)
{
	if (!ChangeNode(static_cast<int>(_nextState), _isForce)) return false;

	// �A�j���[�V�����ɂ��n��
	pAnimation->SetInt(STATE_PARAM_NAME, currentStateKey);
	return true;
}

void BossActionController::OnDamage(const HashiTaku::AttackInformation& _atkInfo)
{
	assert(pCurrentNode && "���݂̃m�[�h���ݒ肳��Ă��܂���");

	// ���݂̃m�[�h�Ƀ_���[�W������`����
	CastBossAct(pCurrentNode)->OnDamage();
}

void BossActionController::SetPlayer(CP_Player& _playerObj)
{
	pPlayerObject = &_playerObj;
}

void BossActionController::SetAttackInfo(const HashiTaku::AttackInformation& _atkInfo)
{
	GetBoss().SetAttackInfo(_atkInfo);
}

CP_Boss& BossActionController::GetBoss()
{
	return static_cast<CP_Boss&>(GetCharacter());
}

CP_Player& BossActionController::GetPlayer()
{
	assert(pPlayerObject && "�v���C���[���ݒ肳��Ă��܂���");

	return *pPlayerObject;
}

const bool BossActionController::GetCanKnock() const
{
	if (!pCanKnock) return false;

	return *pCanKnock;
}

nlohmann::json BossActionController::Save()
{
	auto data = EnemyActionController::Save();
	data["defaultState"] = defaultState;
	return data;
}

void BossActionController::Load(const nlohmann::json& _data)
{
	EnemyActionController::Load(_data);

	bool isLoad = HashiTaku::LoadJsonEnum<BossActState_Base::BossState>(
		"defaultState",
		defaultState,
		_data);

	if (isLoad)
		SetDefaultNode(static_cast<int>(defaultState));
}

void BossActionController::GetAnimationParam()
{
	// �̂�����ł��邩�H
	pCanKnock = pAnimation->GetParameterPointer<bool>(CAN_KNOCK_PARAM_NAME);
}

bool BossActionController::IsCanBossUpdate()
{
#ifdef EDIT
	if (!pPlayerObject)
	{
		return false;
	}
	if (!pAnimation)
	{
		return false;
	}
	if (!pRigidBody)
	{
		return false;
	}
#endif

	return true;
}

BossActState_Base* BossActionController::CastBossAct(HashiTaku::StateNode_Base* _pBaseNode)
{
	if (!_pBaseNode) return nullptr;

	return static_cast<BossActState_Base*>(_pBaseNode);
}

void BossActionController::SetDefaultState(BossActState_Base::BossState _defaultState)
{
	defaultState = _defaultState;
	SetDefaultNode(static_cast<int>(defaultState));
}

std::string BossActionController::GetStateStr(int _stateId)
{
	BossActState_Base::BossState state =
		static_cast<BossActState_Base::BossState>(_stateId);

	return std::string(magic_enum::enum_name<BossActState_Base::BossState>(state));
}

int BossActionController::GetStateId(const std::string& _stateName)
{
	auto state = magic_enum::enum_cast<BossActState_Base::BossState>(_stateName);
	if (!state.has_value())
	{
		assert("����ɏ�Ԃ��擾�ł��܂���");
		return -1;
	}

	return static_cast<int>(state.value());
}

void BossActionController::ImGuiDebug()
{
	// ���O�擾
	std::vector<std::string> stateNames;
	for (auto& node : stateNodeList)
	{

		stateNames.push_back(GetStateStr(node.first));
	}

	// �f�t�H���g��ύX
	std::string stateName = GetStateStr(static_cast<int>(defaultState));
	if (ImGuiMethod::ComboBox("Default", stateName, stateNames))
		SetDefaultState(static_cast<BossActState_Base::BossState>(GetStateId(stateName)));

	// �f�o�b�O�X�e�[�g�ύX
	for (auto& node : stateNodeList)
	{
		if (ImGui::Button(GetStateStr(node.first).c_str()))
			ChangeState(static_cast<BossActState_Base::BossState>(node.first));
	}

	EnemyActionController::ImGuiDebug();
}

