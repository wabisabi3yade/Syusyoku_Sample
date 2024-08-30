#include "pch.h"
#include "AnimationNode_Base.h"

void AnimationNode_Base::ImGuiPlaying()
{
	std::string text = "���O�F";
	text += nodeName;
	ImGui::Text(TO_UTF8(text));

	ImGui::Checkbox("isLoop", &isLoop);

	ImGui::DragFloat("animationTime", &animationTime, 0.1f, 0.0f, 100.0f);

	ImGui::Text(TO_UTF8("�J�ڐ�"));
	// �J�ڐ�̃A�j���[�V����
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
	// ���^�C�~���O�ŕ����̏������B�������ꍇ�A�o�^���ɂȂ�

	// �����B�����Ă���m�F
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

	if (isLoop)	// ���[�v�Đ��ɕς���Ȃ�I���t���O�����낷
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
