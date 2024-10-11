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
	: nodeName(_nodeName), nodeType(_type), curPlayingRatio(0.0f), lastPlayingRatio(0.0f), playNodeSpeedTimes(1.0f), animationTime(1.0f), isLoop(true), isFinish(false), isRootMotionPosXZ(false), isRootMotionPosY(false), isRootMotionRot(false)
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

	// 1フレーム前を現在の少し前に設定しておく
	lastPlayingRatio = curPlayingRatio - Mathf::smallValue;
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

const std::string& AnimationNode_Base::GetNodeName() const
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

float AnimationNode_Base::GetLastPlayRatio() const
{
	return lastPlayingRatio;
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

bool AnimationNode_Base::GetIsRootMotionXZ() const
{
	return isRootMotionPosXZ;
}

bool AnimationNode_Base::GetIsRootMotionY() const
{
	return isRootMotionPosY;
}

bool AnimationNode_Base::GetIsRootMotionRot() const
{
	return isRootMotionRot;
}

float AnimationNode_Base::GetPlaySpeedTimes() const
{
	return playNodeSpeedTimes;
}

nlohmann::json AnimationNode_Base::Save()
{
	// ノード名とタイプはコントローラーで管理
	nlohmann::json nodeData;
	nodeData["animTime"] = animationTime;
	nodeData["isLoop"] = isLoop;
	nodeData["isRMXZ"] = isRootMotionPosXZ;
	nodeData["isRMY"] = isRootMotionPosY;
	nodeData["isRMR"] = isRootMotionRot;
	return nodeData;
}

void AnimationNode_Base::Load(const nlohmann::json& _data)
{
	LoadJsonFloat("animTime", animationTime, _data);
	LoadJsonBoolean("isLoop", isLoop, _data);
	LoadJsonBoolean("isRMXZ", isRootMotionPosXZ, _data);
	LoadJsonBoolean("isRMY", isRootMotionPosY, _data);
	LoadJsonBoolean("isRMR", isRootMotionRot, _data);
}

void AnimationNode_Base::ProgressPlayRatio(float _controllerSpeed)
{
	// 1フレーム前の再生割合を更新
	lastPlayingRatio = curPlayingRatio;

	// 時間を進める
	float progressRatioSpeed = 1.0f / animationTime;
	curPlayingRatio += progressRatioSpeed * _controllerSpeed * playNodeSpeedTimes *MainApplication::DeltaTime();

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
	char buf[IM_INPUT_BUF];	// 変数名を変更
	strncpy_s(buf, nodeName.c_str(), sizeof(buf));

	if (ImGui::InputText("##name", buf, IM_INPUT_BUF, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		nodeName = buf;
	}

	std::vector<std::string>& ntStrings = AnimationNode_Base::edit_nodeTypeStrings;
	const std::string& typeStr = ntStrings[static_cast<u_int>(nodeType)];
	ImGui::Text(std::string("NodeType:" + typeStr).c_str());	// ノードタイプ

	ImGui::Checkbox("IsLoop", &isLoop);
	ImGui::Text("RootMotion");
	ImGui::Checkbox("XZ", &isRootMotionPosXZ); ImGui::SameLine();
	ImGui::Checkbox("Y", &isRootMotionPosY); ImGui::SameLine();
	ImGui::Checkbox("Rot", &isRootMotionRot);

	ImGuiMethod::PushItemWidth();
	ImGui::DragFloat("Speed", &playNodeSpeedTimes, 0.01f, 0.0f, 100.0f);
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
