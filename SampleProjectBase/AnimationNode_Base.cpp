#include "pch.h"
#include "AnimationNode_Base.h"

using namespace HashiTaku;

#ifdef EDIT
std::vector<std::string> AnimationNode_Base::edit_nodeTypeStrings =
{
	"Single",
	"Blend",
	"Layerd"
};
#endif // EDIT

AnimationNode_Base::AnimationNode_Base(const std::string& _nodeName, NodeType _type)
	: nodeName(_nodeName),
	nodeType(_type), 
	playNodeSpeedTimes(1.0f),
	animationTime(1.0f), 
	isLoop(true), 
	isFinish(false),
	isRootMotionPosXZ(false),
	isRootMotionPosY(false)
{
	pCurveSpeed = std::make_unique<AnimationCurve>();
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

float AnimationNode_Base::GetAnimationTime() const
{
	return animationTime;
}

u_int AnimationNode_Base::GetAllKeyFrame() const
{
	// アニメーションデータがないので仮に1
	return 1;
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

float AnimationNode_Base::GetPlaySpeedTimes() const
{
	return playNodeSpeedTimes;
}

float AnimationNode_Base::GetCurveValue(float _ratio) const
{
	return pCurveSpeed->GetValue(_ratio);
}

json AnimationNode_Base::Save()
{
	// ノード名とタイプはコントローラーで管理
	json nodeData;
	nodeData["speed"] = playNodeSpeedTimes;
	nodeData["isLoop"] = isLoop;
	nodeData["isRMXZ"] = isRootMotionPosXZ;
	nodeData["isRMY"] = isRootMotionPosY;
	nodeData["animationCurve"] = pCurveSpeed->Save();
	return nodeData;
}

void AnimationNode_Base::Load(const json& _data)
{
	LoadJsonFloat("speed", playNodeSpeedTimes, _data);
	LoadJsonBoolean("isLoop", isLoop, _data);
	LoadJsonBoolean("isRMXZ", isRootMotionPosXZ, _data);
	LoadJsonBoolean("isRMY", isRootMotionPosY, _data);

	json loadData;
	if (LoadJsonData("animationCurve", loadData, _data))
	{
		pCurveSpeed->Load(loadData);
	}
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

	ImGuiMethod::PushItemSmallWidth();
	ImGui::DragFloat("Speed", &playNodeSpeedTimes, 0.01f, 0.0f, 100.0f);
	pCurveSpeed->ImGuiCall();
	ImGui::PopItemWidth();
#endif // EDIT
}

void AnimationNode_Base::SetAnimationTime(float _time)
{
	animationTime = std::max(_time, 0.0f);
}

void AnimationNode_Base::ImGuiDebug()
{
	ImGuiSetParameter();
}
