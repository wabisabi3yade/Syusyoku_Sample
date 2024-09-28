#include "pch.h"
#include "PlayerAnimController.h"

#include "SingleAnimationNode.h"
#include "BlendAnimationNode.h"

#include "GameInput.h"

using namespace HashiTaku;

PlayerAnimController::PlayerAnimController() : nowState(AnimType::Move)
{
	Init();
}

void PlayerAnimController::SetStartAnimation(AnimType _type)
{
	assert(_type < AnimType::MaxNum);

	nowState = _type;
	std::string nodeName = animTypeNodeNames[_type];
	AnimationController::SetDefaultNode(nodeName);
}

void PlayerAnimController::SetMoveSpeedRatio(float _speedRatio)
{
	speedRatio = _speedRatio;

	// 移動アニメーションのブレンド割合をセット
	if (nowState != AnimType::Move) return;
	SetBlendRatio(_speedRatio);
}

void PlayerAnimController::Init()
{
	// ノード作成
	InitCreateNode();

	InitTransitionArrow();

	// 初期アニメーションセット
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
	AnimationNode_Base* pAttack = GetNodeInfo(attackNodeName)->pAnimNode.get();
	pAttack->SetIsLoop(false);

	// 名前と列挙型対応
	using enum AnimType;
	LinkAnimTypeNodeName(Move, moveNodeName);
	LinkAnimTypeNodeName(Attack, attackNodeName);
}

void PlayerAnimController::InitTransitionArrow()
{
	using enum AnimType;

	//// Move
	//AnimTransitionArrow* pMoveToAtk = CreateTransitionArrow(animTypeNodeNames[Move], animTypeNodeNames[Attack], 0.2f, 0.2f,
	//	[]()
	//	{
	//		return GameInput::GetInstance()->GetButtonDown(GameInput::ButtonType::Player_Attack);
	//	});
	//pMoveToAtk->SetEaseKind(EaseKind::OutBack);


	//// Attack
	//AnimationNode_Base* pAttackNode = GetNodeInfo(animTypeNodeNames[Attack])->pAnimNode.get();
	//AnimTransitionArrow* pAtkToMove = CreateTransitionArrow(animTypeNodeNames[Attack], animTypeNodeNames[Move], 0.1f, 0.2f, [pAttackNode]()
	//	{
	//		return pAttackNode->GetIsFinish();
	//	});

	//pAtkToMove->SeInterpolateKind(HashiTaku::AnimInterpolateKind::Inertialization);
}

void PlayerAnimController::LinkAnimTypeNodeName(AnimType _animType, const std::string& _nodeName)
{
	animTypeNodeNames[_animType] = _nodeName;
}

void PlayerAnimController::OnAnimationFinish()
{
	AnimationController::OnAnimationFinish();

	// 終了したノード名から列挙型を取得
	AnimType finType = FindAnimType(pCurrentNodeInfo->pAnimNode->GetNodeName());

	// 登録しているオブザーバーに送信
	NotifyFinish(static_cast<u_int>(finType));
}

void PlayerAnimController::ChangeAnimation(const std::string& _animName, float _targetAnimRatio, bool _isInterp)
{
	AnimationController::ChangeAnimation(_animName);
	nowState = FindAnimType(pCurrentNodeInfo->pAnimNode->GetNodeName());
}

PlayerAnimController::AnimType PlayerAnimController::FindAnimType(const std::string& _nodeName)
{
	// 終了したノード名から列挙型を取得
	AnimType findType = AnimType::MaxNum;
	for (auto& link : animTypeNodeNames)
	{
		if (link.second == _nodeName)
		{
			findType = link.first;
			break;
		}
	}
	assert(findType != AnimType::MaxNum && "ノード名から列挙型を探せませんでした");

	return findType;
}

void PlayerAnimController::ImGuiSetting()
{
	AnimationController::ImGuiSetting();

	ImGui::Text(std::string("AnimationState:" + animTypeNodeNames[nowState]).c_str());
}
