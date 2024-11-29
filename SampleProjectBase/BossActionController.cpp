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

#ifdef EDIT
#include "Geometory.h"
#endif // EDIT

namespace DXSimp = DirectX::SimpleMath;

// ��Ԃ�\���A�j���[�V�����p�����[�^��
constexpr auto STATE_PARAM_NAME("state");
// �̂����肪�ł��邩�A�j���[�V�����p�����[�^��
constexpr auto CAN_KNOCK_PARAM_NAME("canKnock");

BossActionController::BossActionController(CP_Boss& _boss) :
	EnemyActionController(_boss, "bossActCon"), pPlayerObject(nullptr), pCanKnock(nullptr),
	defaultState(BossActState_Base::BossState::Idle)
{
	// ������
	disLengthList = { 0.0f };

	// �X�e�[�g�쐬
	using enum BossActState_Base::BossState;
	using enum ActDistance;
	CreateState<BossIdleState>(Idle, {});
	CreateState<BossGroundMove>(Walk, { ActDistance::Mid });
	CreateState<BossGroundMove>(Run, { ActDistance::Far });

	CreateState<BossDamageState>(Damage_Small, {});

	CreateState<BossCombAttack>(CombAttack1, { ActDistance::Short });
	CreateState<BossJumpAttack>(JumpAttack, { ActDistance::Far });

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
	// �X�V�ł��邩
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

void BossActionController::OnDamage(const HashiTaku::AttackInformation& _atkInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos, bool* _pAcceptDamage)
{
	assert(pCurrentNode && "���݂̃m�[�h���ݒ肳��Ă��܂���");

	// ���݂̃m�[�h�Ƀ_���[�W������`����
	CastBossAct(pCurrentNode)->OnDamage();
}

void BossActionController::DebugDisplay()
{
#ifdef EDIT
	EnemyActionController::DebugDisplay();

	if (!pPlayerObject) return;

	// �ǂ̋������\������
	DXSimp::Vector3 bossPos = GetBoss().GetTransform().GetPosition();
	DXSimp::Vector3 playerPos = pPlayerObject->GetTransform().GetPosition();
	float dis = (bossPos - playerPos).Length();
	ActDistance curDis = JudgeActDistance(dis);

	// ���̐F�ŕ\��
	DirectX::SimpleMath::Color lineColor = DXSimp::Color(1, 0, 0);
	switch (curDis)
	{
	case ActDistance::Mid:
		lineColor = DXSimp::Color(1, 1, 0);
		break;
	case ActDistance::Far:
		lineColor = DXSimp::Color(0, 1, 0);
		break;
	}

	Geometory::AddLine(bossPos, playerPos, lineColor);
#endif // EDIT
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

float BossActionController::GetActDistanceLength(ActDistance _actDistance)
{
	return disLengthList[static_cast<u_int>(_actDistance)];
}

BossActionController::ActDistance BossActionController::JudgeActDistance(float _disLength) const
{
	u_int disCnt = static_cast<u_int>(ActDistance::MaxNum);
	ActDistance curDisType = static_cast<ActDistance>(disCnt - 1);	// �Œ����Z�b�g���Ă���

	// �ǂ̋���������
	for (u_int d_i = 0; d_i < disCnt - 1; d_i++)
	{
		ActDistance disType = static_cast<ActDistance>(d_i);
		float dis = disLengthList[d_i];

		// �������͈͊O�Ȃ�
		if (dis < _disLength) continue;

		curDisType = disType;
		break;
	}

	return curDisType;
}

nlohmann::json BossActionController::Save()
{
	auto data = EnemyActionController::Save();
	data["defaultState"] = defaultState;

	// �������Z�[�u
	auto& disLengthDatas = data["disLengthList"];
	for (auto& disLength : disLengthList)
	{
		disLengthDatas.push_back(disLength);
	}
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

	// ���������[�h
	nlohmann::json disDatas;
	if (HashiTaku::LoadJsonDataArray("disLengthList", disDatas, _data))
	{
		u_int i = 0;
		for (auto& disData : disDatas)
		{
			disLengthList[i] = disData;
			i++;
		}
	}
}

BossActState_Base::BossState BossActionController::RandState(ActDistance _actDistance)
{
	// �����̃X�e�[�g���X�g�����炤
	std::vector<BossActState_Base::BossState>* pActLists = &shortRangeActions;
	switch (_actDistance)
	{
	case ActDistance::Mid:
		pActLists = &midRangeActions;
		break;

	case ActDistance::Far:
		pActLists = &farRangeActions;
		break;
	}

	// ���Ɍ��܂�悤�Ȏ��ۂȂ�
	u_int stateCnt = static_cast<u_int>(pActLists->size());
	if (stateCnt == 0)
	{
		std::string disName = std::string(magic_enum::enum_name<ActDistance>(_actDistance));
		HASHI_DEBUG_LOG(disName + "�̃X�e�[�g���������܂���");
		return BossActState_Base::BossState::Run;
	}
	else if (stateCnt == 1)	// �X�e�[�g����Ȃ�
	{
		return (*pActLists)[0];
	}

	// �m���ŃX�e�[�g��I��
	float rand = HashiTaku::Random::Range<float>(0.0f, 1.0f);
	float oneStateProbability = 1.0f / stateCnt;
	u_int arrayId = static_cast<u_int>(rand / oneStateProbability);

	return (*pActLists)[arrayId];
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

	// �e�����̒���
	if (ImGuiMethod::TreeNode("Distance"))
	{
		u_int disCnt = static_cast<u_int>(ActDistance::MaxNum);
		for (u_int d_i = 0; d_i < disCnt; d_i++)
		{
			std::string disName =
				std::string(magic_enum::enum_name<ActDistance>(static_cast<ActDistance>(d_i)));

			ImGui::DragFloat(disName.c_str(), &disLengthList[d_i], 0.01f, 0.0f, 1000.0f);
		}

		ImGui::TreePop();
	}

	// �f�t�H���g��ύX
	std::string stateName = GetStateStr(static_cast<int>(defaultState));
	if (ImGuiMethod::ComboBox("Default", stateName, stateNames))
		SetDefaultState(static_cast<BossActState_Base::BossState>(GetStateId(stateName)));
	EnemyActionController::ImGuiDebug();

	ImGui::PushID(9);
	// �f�o�b�O�X�e�[�g�ύX
	for (auto& node : stateNodeList)
	{
		if (ImGui::Button(GetStateStr(node.first).c_str()))
			ChangeState(static_cast<BossActState_Base::BossState>(node.first));
	}
	ImGui::PopID();
}

