#include "pch.h"
#include "PlayerAnimController.h"

#include "BlendAnimationNode.h"

#include "AssetGetter.h"

PlayerAnimController::PlayerAnimController()
{
	std::unique_ptr<BlendAnimationNode> blendNode = std::make_unique<BlendAnimationNode>("Move");

	pCurrentAnimNode = blendNode.get();

	AnimationData* pAnimData = AssetGetter::GetAsset<AnimationData>("Idle");
	blendNode->SetAnimationData(*pAnimData);

	pAnimData = AssetGetter::GetAsset<AnimationData>("Walk_Forward");
	blendNode->SetAnimationData(*pAnimData);

	pAnimData = AssetGetter::GetAsset<AnimationData>("Run_Forward");
	blendNode->SetAnimationData(*pAnimData);

	pAnimationNodes["Move"] = std::move(blendNode);
}
