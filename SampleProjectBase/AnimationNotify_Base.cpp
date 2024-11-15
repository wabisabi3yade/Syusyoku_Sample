#include "pch.h"
#include "AnimationNotify_Base.h"

// フレームでなく割合で調整するときのアニメーションのフレーム数
constexpr u_int DONT_USE_FRAME_CNT(1);

AnimationNotify_Base::AnimationNotify_Base()
	: isActive(true), notifyName(""), notifyType(NotifyType::None), animFrameCnt(0)
{
}

AnimationNotify_Base::AnimationNotify_Base(NotifyType _notifyType)
	: notifyType(_notifyType), animFrameCnt(0), isActive(true), notifyName("")
{
}

void AnimationNotify_Base::Init()
{
}

void AnimationNotify_Base::OnInitCall()
{
	if (!isActive) return;
	OnInitialize();
}

void AnimationNotify_Base::OnTerminalCall()
{
	if (!isActive) return;
	OnTerminal();
}

void AnimationNotify_Base::SetIsActive(bool _isActive)
{
	isActive = _isActive;
}

void AnimationNotify_Base::SetNotifyName(const std::string& _notifyName)
{
	notifyName = _notifyName;
}

void AnimationNotify_Base::SetAnimationFrameCnt(u_int _animFrame)
{
	animFrameCnt = _animFrame;
}

std::string AnimationNotify_Base::GetTypeToStr(NotifyType _notifyType)
{
	switch (_notifyType)
	{
	case AnimationNotify_Base::NotifyType::ANE_DebugLog:
		return "ANE_DebugLog";
		break;
	case AnimationNotify_Base::NotifyType::ANS_DebugLog:
		return "ANS_DebugLog";
		break;
	case AnimationNotify_Base::NotifyType::ANE_ChangeParameter:
		return "ANE_ChangeParameter";
		break;
	case AnimationNotify_Base::NotifyType::ANS_ChangeParameter:
		return "ANS_ChangeParameter";
		break;
	default:
		break;
	}

	return "";
}

bool AnimationNotify_Base::GetIsActive() const
{
	return isActive;
}

std::string AnimationNotify_Base::GetNotifyName() const
{
	return notifyName;
}

AnimationNotify_Base::NotifyType AnimationNotify_Base::GetNotifyType() const
{
	return notifyType;
}

u_int AnimationNotify_Base::GetAnimFrameCnt() const
{
	return animFrameCnt;
}

float AnimationNotify_Base::ConvertFrameToRatio(u_int _frame)
{
	// フレーム数を使用していないなら
	if (animFrameCnt <= DONT_USE_FRAME_CNT)
	{
		return 0.0f;
	}
	// 割合を求める
	float ratio = static_cast<float>(_frame) / animFrameCnt;

	return std::clamp(ratio, 0.0f, 1.0f);
}

bool AnimationNotify_Base::UseFrame()
{
	return animFrameCnt != DONT_USE_FRAME_CNT;
}

nlohmann::json AnimationNotify_Base::Save()
{
	nlohmann::json data;
	data["isActive"] = isActive;
	data["notifyName"] = notifyName;
	return data;
}

void AnimationNotify_Base::Load(const nlohmann::json& _data)
{
	HashiTaku::LoadJsonBoolean("isActive", isActive, _data);
	HashiTaku::LoadJsonString("notifyName", notifyName, _data);
}

void AnimationNotify_Base::ImGuiDebug()
{
#ifdef EDIT
	ImGui::Checkbox("isActive", &isActive);
	ImGuiMethod::EditableText("##anb", notifyName, isEditing);
#endif // EDIT
}
