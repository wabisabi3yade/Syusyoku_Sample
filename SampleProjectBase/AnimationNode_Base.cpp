#include "pch.h"
#include "AnimationNode_Base.h"

using namespace HashiTaku;

#ifdef EDIT
std::vector<std::string> AnimationNode_Base::edit_nodeTypeStrings =
{
		"Single",
		"Blend"
};
#endif // EDIT

AnimationNode_Base::AnimationNode_Base(std::string _nodeName, NodeType _type)
	: nodeName(_nodeName), nodeType(_type), curPlayingRatio(0.0f), playSpeed(1.0f), animationTime(0.0f), isLoop(true), isFinish(false)
{
}

AnimationNode_Base::AnimationNode_Base(const AnimationNode_Base& _other)
{
	Copy(_other);
}

AnimationNode_Base& AnimationNode_Base::operator=(const AnimationNode_Base& _other)
{
	Copy(_other);

	return *this;
}

void AnimationNode_Base::ImGuiPlaying()
{
#ifdef EDIT
	ImGui::Text(std::string("NodeName:" + nodeName).c_str());
	ImGui::SliderFloat("PlayRatio", &curPlayingRatio, 0.0f, 1.0f);
#endif // EDIT
}

void AnimationNode_Base::Begin()
{
	isFinish = false;
}

void AnimationNode_Base::UpdateCall(BoneList& _boneList)
{
	Update(_boneList);
}

void AnimationNode_Base::SetCurPlayRatio(float _playingRatio)
{
	curPlayingRatio = _playingRatio;
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

std::string AnimationNode_Base::GetNodeName() const
{
	return nodeName;
}

AnimationNode_Base::NodeType AnimationNode_Base::GetNodeType() const
{
	return nodeType;
}

float AnimationNode_Base::GetCurPlayRatio() const
{
	return curPlayingRatio;
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

nlohmann::json AnimationNode_Base::Save()
{
	// ノード名とタイプはコントローラーで管理
	nlohmann::json nodeData;
	nodeData["animTime"] = animationTime;
	nodeData["isLoop"] = isLoop;
	return nodeData;
}

void AnimationNode_Base::Load(const nlohmann::json& _data)
{
	LoadJsonFloat("animTime", animationTime, _data);
	LoadJsonBoolean("isLoop", isLoop, _data);
}

void AnimationNode_Base::ProgressPlayRatio(float _playSpeed)
{
	float progressRatioSpeed = 1.0f / animationTime;

	// 時間を進める
	curPlayingRatio += progressRatioSpeed * _playSpeed * MainApplication::DeltaTime();

	if (IsCanLoop())	// ループできるなら
		curPlayingRatio -= 1.0f;
}

bool AnimationNode_Base::IsCanLoop()
{
	// アニメーションの全体時間を超えていないなら
	if (curPlayingRatio < 1.0f) return false;
	if (!isLoop)
	{
		SetFinish();	// 終了処理
	}

	return true;
}

void AnimationNode_Base::Copy(const AnimationNode_Base& _other)
{
	if (this == &_other) return;

}

void AnimationNode_Base::ImGuiSetParameter()
{
#ifdef EDIT
	std::vector<std::string>& ntStrings = AnimationNode_Base::edit_nodeTypeStrings;
	const std::string& typeStr = ntStrings[static_cast<u_int>(nodeType)];
	ImGui::Text(std::string("NodeType:" + typeStr).c_str());	// ノードタイプ

	ImGui::Checkbox("IsLoop", &isLoop);

	ImGuiMethod::PushItemWidth();
	ImGui::DragFloat("Speed", &playSpeed, 1.0f, 0.0f, 100.0f);
	ImGui::PopItemWidth();

	ImGui::Text("AnimationTime:%f", animationTime);
#endif // EDIT
}

void AnimationNode_Base::SetAnimationTime(float _time)
{
	animationTime = std::max(_time, 0.0f);
}

void AnimationNode_Base::ImGuiSetting()
{
	ImGuiSetParameter();
}
