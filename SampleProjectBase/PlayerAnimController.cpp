#include "pch.h"
#include "PlayerAnimController.h"

#include "SingleAnimationNode.h"
#include "BlendAnimationNode.h"

#include "GameInput.h"

PlayerAnimController::PlayerAnimController()
	: nowState(AnimType::Move)
{
	Init();
}

void PlayerAnimController::SetStartAnimation(AnimType _type)
{
	assert(_type < AnimType::MaxNum);

	nowState = _type;
	std::string nodeName = animTypeNodeNames[_type];
	AnimationController::ChangeAnimation(nodeName, 0.0f, false);
}

void PlayerAnimController::SetMoveSpeedRatio(float _speedRatio)
{
	speedRatio = _speedRatio;

	// �ړ��A�j���[�V�����̃u�����h�������Z�b�g
	if (nowState != AnimType::Move) return;
	SetBlendRatio(_speedRatio);
}

void PlayerAnimController::Init()
{
	// �m�[�h�쐬
	InitCreateNode();

	InitTransitionArrow();

	// �����A�j���[�V�����Z�b�g
	SetStartAnimation(AnimType::Move);
}

void PlayerAnimController::InitCreateNode()
{
	// Move
	std::vector<std::string> animNames =
	{
		"Idle.fbx",
		"WalkF.fbx",
		"RunF.fbx"
	};
	std::vector<float> ratios =
	{
		0.0f,
		0.5f,
		1.0f
	};
	std::string moveNodeName = "Move";
	CreateBlendNode(animNames, ratios, moveNodeName);

	// AttackN1
	std::string attackNodeName = "AttackN1";
	CreateSingleNode(attackNodeName, "AttackN1.fbx");
	AnimationNode_Base* pAttack = GetNode(attackNodeName);
	pAttack->SetIsLoop(false);

	// ���O�Ɨ񋓌^�Ή�
	using enum AnimType;
	LinkAnimTypeNodeName(Move, moveNodeName);
	LinkAnimTypeNodeName(Attack, attackNodeName);
}

void PlayerAnimController::InitTransitionArrow()
{
	using enum AnimType;

	// Move
	CreateTransitionArrow(animTypeNodeNames[Move], animTypeNodeNames[Attack], 0.0f, 0.2f,
		[]()
		{
			return GameInput::GetInstance()->GetButtonDown(GameInput::ButtonType::Player_Attack);
		});

	// Attack
	AnimationNode_Base* pAttackNode = GetNode(animTypeNodeNames[Attack]);
	CreateTransitionArrow(animTypeNodeNames[Attack], animTypeNodeNames[Move], 0.1f, 0.2f, [pAttackNode]()
		{
			return pAttackNode->GetIsFinish();
		});
}

void PlayerAnimController::LinkAnimTypeNodeName(AnimType _animType, const std::string& _nodeName)
{
	animTypeNodeNames[_animType] = _nodeName;
}

void PlayerAnimController::OnAnimationFinish()
{
	AnimationController::OnAnimationFinish();

	// �I�������m�[�h������񋓌^���擾
	AnimType finType = FindAnimType(pCurrentAnimNode->GetNodeName());

	// �o�^���Ă���I�u�U�[�o�[�ɑ��M
	NotifyFinish(static_cast<u_int>(finType));
}

void PlayerAnimController::ChangeAnimation(const std::string& _animName, float _targetAnimRatio, bool _isInterp)
{
	AnimationController::ChangeAnimation(_animName, _targetAnimRatio, _isInterp);

	nowState = FindAnimType(pCurrentAnimNode->GetNodeName());
}

PlayerAnimController::AnimType PlayerAnimController::FindAnimType(const std::string& _nodeName)
{
	// �I�������m�[�h������񋓌^���擾
	AnimType findType = AnimType::MaxNum;
	for (auto& link : animTypeNodeNames)
	{
		if (link.second == _nodeName)
		{
			findType = link.first;
			break;
		}
	}
	assert(findType != AnimType::MaxNum && "�m�[�h������񋓌^��T���܂���ł���");

	return findType;
}

void PlayerAnimController::ImGuiSetting()
{
	AnimationController::ImGuiSetting();

	ImGui::Text(std::string("AnimationState:" + animTypeNodeNames[nowState]).c_str());
}
