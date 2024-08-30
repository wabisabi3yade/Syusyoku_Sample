#include "pch.h"
#include "PlayerAnimController.h"

#include "SingleAnimationNode.h"
#include "BlendAnimationNode.h"

#include "GameInput.h"

PlayerAnimController::PlayerAnimController()
{
	Init();
}

void PlayerAnimController::ChangeAnimationByState(AnimType _type, bool _isIneterp)
{
	nowState = _type;

	using enum AnimType;
	switch (_type)
	{
	case Move:
		AnimationController::ChangeAnimationStart("Move", 0.0f, _isIneterp);
		break;

	case Jump:
		AnimationController::ChangeAnimationStart("Jump", 0.0f, _isIneterp);
		break;

	case Attack:
		AnimationController::ChangeAnimationStart("AttackN1", 0.0f, _isIneterp);
		break;

	default:
		break;
	}
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

	CreateBlendNode(animNames, ratios, "Move");

	// 攻撃
	CreateSingleNode("AttackN1", "AttackN1.fbx");
	AnimationNode_Base* pAttack = GetNode("AttackN1");
	pAttack->SetIsLoop(false);

	ChangeAnimationByState(AnimType::Move, false);

	CreateTransitionArrow("Move", "AttackN1", 0.5f, []()
		{
			return GameInput::GetInstance()->GetButtonDown(GameInput::ButtonType::Player_Attack);
		});

	CreateTransitionArrow("AttackN1", "Move", 0.2f, [pAttack]()
		{
			return pAttack->GetIsFinish();
		});
}

void PlayerAnimController::OnAnimationFinish()
{
	AnimationController::OnAnimationFinish();

	u_int stateNum = static_cast<u_int>(nowState);
	NotifyFinish(stateNum);
}
