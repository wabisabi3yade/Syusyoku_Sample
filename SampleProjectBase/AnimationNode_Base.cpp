#include "pch.h"
#include "AnimationNode_Base.h"

void AnimationNode_Base::ImGuiPlaying()
{
	std::string text = "名前：";
	text += nodeName;
	ImGui::Text(TO_UTF8(text));

	ImGui::Checkbox("isLoop", &isLoop);

	ImGui::DragFloat("animationTime", &animationTime, 0.1f, 0.0f, 100.0f);

	ImGui::Text(TO_UTF8("遷移先"));
	// 遷移先のアニメーション
	for (auto& arrow : pFromArrows)
	{
		arrow->ImGuiCall();
	}
}

void AnimationNode_Base::Begin()
{
	isFinish = false;
}

void AnimationNode_Base::UpdateCall(float _playingRatio, BoneList& _boneList)
{
	Update(_playingRatio, _boneList);
}

AnimTransitionArrow* AnimationNode_Base::CheckTransition()
{
	// 同タイミングで複数の条件が達成した場合、登録順になる

	// 条件達成している確認
	for (auto& arrow : pFromArrows)
	{
		if (!arrow->CheckTransition()) continue;

		return arrow.get();
	}

	return nullptr;
}

void AnimationNode_Base::SetNodeName(const std::string& _nodeName)
{
	nodeName = _nodeName;
}

void AnimationNode_Base::SetIsLoop(bool _isLoop)
{
	isLoop = _isLoop;

	if (isLoop)	// ループ再生に変えるなら終了フラグをおろす
		isFinish = false;
}

void AnimationNode_Base::SetFinish()
{
	isFinish = true;
}

void AnimationNode_Base::AddTransitionArrow(std::unique_ptr<AnimTransitionArrow> _setArrow)
{
	pFromArrows.push_back(std::move(_setArrow));
}

std::string AnimationNode_Base::GetNodeName() const
{
	return nodeName;
}

AnimationNode_Base::NodeType AnimationNode_Base::GetNodeType() const
{
	return nodeType;
}

float AnimationNode_Base::GetAnimationTime() const
{
	return animationTime;
}

bool AnimationNode_Base::GetIsLoop() const
{
	return isLoop;
}

bool AnimationNode_Base::GetIsFinish() const
{
	return isFinish;
}

void AnimationNode_Base::SetAnimationTime(float _time)
{
	animationTime = std::max(_time, 0.0f);
}
