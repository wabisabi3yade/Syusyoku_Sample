#include "pch.h"
#include "Easing.hpp"

#include "EaseFunction.hpp"

using namespace HashiTaku;

// static変数初期化
std::unordered_map<EaseKind, std::unique_ptr<IEasing>> Easing::easeList;
std::vector<std::string> Easing::easeNames;

float HashiTaku::Easing::EaseValue(float _ratio, EaseKind _easeType)
{
	return easeList[_easeType]->Get(_ratio);
}

bool HashiTaku::Easing::ImGuiSelect(EaseKind& _nowEase)
{
#ifdef EDIT
	u_int easeId = static_cast<u_int>(_nowEase);

	if (ImGuiMethod::ComboBox("Easing", easeId, easeNames))
	{
		_nowEase = static_cast<EaseKind>(easeId);
		return true;
	}
#endif // EDIT
	return false;
}

void HashiTaku::Easing::Init()
{
	// イージングクラスのオブジェクトをを作成
	for (u_int e_i = 0; e_i < static_cast<u_int>(EaseKind::MaxNum); e_i++)
		MakeEase(e_i);
}

void HashiTaku::Easing::MakeEase(u_int _easeId)
{
	std::string easeName;
	std::unique_ptr<IEasing> easeClass;

	EaseKind ease = static_cast<EaseKind>(_easeId);

	switch (ease)
	{
	case EaseKind::Linear:
		easeClass = std::make_unique<Ease_Linear>();
		easeName = "Linear"; break;

	case EaseKind::InSine:
		easeClass = std::make_unique<Ease_InSine>();
		easeName = "In_Sine"; break;

	case EaseKind::OutSine:
		easeClass = std::make_unique<Ease_OutSine>();
		easeName = "Out_Sine"; break;

	case EaseKind::InOutSine:
		easeClass = std::make_unique<Ease_InSine>();
		easeName = "InOut_Sine"; break;

	case EaseKind::InQuad: easeClass = std::make_unique<Ease_InQuad>();
		easeName = "In_Quad"; break;

	case EaseKind::OutQuad: easeClass = std::make_unique<Ease_OutQuad>();
		easeName = "Out_Quad"; break;

	case EaseKind::InOutQuad: easeClass = std::make_unique<Ease_InOutQuad>();
		easeName = "InOut_Quad"; break;

	case EaseKind::InCubic:  easeClass = std::make_unique<Ease_InCubic>();
		easeName = "In_Cubic"; break;

	case EaseKind::OutCubic:  easeClass = std::make_unique<Ease_OutCubic>();
		easeName = "Out_Cubic"; break;

	case EaseKind::InOutCubic:  easeClass = std::make_unique<Ease_InOutCubic>();
		easeName = "InOut_Cubic"; break;

	case EaseKind::InQuart:  easeClass = std::make_unique<Ease_InQuart>();
		easeName = "In_Quart"; break;

	case EaseKind::OutQuart:  easeClass = std::make_unique<Ease_OutQuart>();
		easeName = "Out_Quart"; break;

	case EaseKind::InOutQuart:  easeClass = std::make_unique<Ease_InOutQuart>();
		easeName = "InOut_Quart"; break;

	case EaseKind::InQuint:  easeClass = std::make_unique<Ease_InQuint>();
		easeName = "In_Quint"; break;

	case EaseKind::OutQuint:  easeClass = std::make_unique<Ease_OutQuint>();
		easeName = "Out_Quint"; break;

	case EaseKind::InOutQuint:  easeClass = std::make_unique<Ease_InOutQuint>();
		easeName = "InOut_Quint"; break;

	case EaseKind::InExpo:  easeClass = std::make_unique<Ease_InExpo>();
		easeName = "In_Expo"; break;

	case EaseKind::OutExpo:  easeClass = std::make_unique<Ease_OutExpo>();
		easeName = "Out_Expo"; break;

	case EaseKind::InOutExpo:  easeClass = std::make_unique<Ease_InOutExpo>();
		easeName = "InOut_Expo"; break;

	case EaseKind::InCirc:  easeClass = std::make_unique<Ease_InCirc>();
		easeName = "In_Circ"; break;

	case EaseKind::OutCirc:  easeClass = std::make_unique<Ease_OutCirc>();
		easeName = "Out_Circ"; break;

	case EaseKind::InOutCirc:  easeClass = std::make_unique<Ease_InOutCirc>();
		easeName = "InOut_Circ"; break;

	case EaseKind::InBack:  easeClass = std::make_unique<Ease_InBack>();
		easeName = "In_Back"; break;

	case EaseKind::OutBack:  easeClass = std::make_unique<Ease_OutBack>();
		easeName = "Out_Back"; break;

	case EaseKind::InOutBack:  easeClass = std::make_unique<Ease_InOutBack>();
		easeName = "InOut_Back"; break;

	case EaseKind::InElastic:  easeClass = std::make_unique<Ease_InElastic>();
		easeName = "In_Elastic"; break;

	case EaseKind::OutElastic:  easeClass = std::make_unique<Ease_OutElastic>();
		easeName = "Out_Elastic"; break;

	case EaseKind::InOutElastic:  easeClass = std::make_unique<Ease_InOutElastic>();
		easeName = "InOut_Elastic"; break;

	case EaseKind::InBounce:  easeClass = std::make_unique<Ease_InBounce>();
		easeName = "In_Bounce"; break;

	case EaseKind::OutBounce:  easeClass = std::make_unique<Ease_OutBounce>();
		easeName = "Out_Bounce"; break;

	case EaseKind::InOutBounce:  easeClass = std::make_unique<Ease_InOutBounce>();
		easeName = "InOut_Bounce"; break;

	default:
		HASHI_DEBUG_LOG("イージング作成失敗");
		break;
	}

	easeList[ease] = std::move(easeClass);
	easeNames.push_back(easeName);
}

EaseKind HashiTaku::Easing::StrToEase(const std::string& _easeName)
{
	for (u_int n_i = 0; n_i < static_cast<u_int>(EaseKind::MaxNum); n_i++)
	{
		if (easeNames[n_i] == _easeName)
			return static_cast<EaseKind>(n_i);
	}

	assert(!"イージング名がありません");
	return EaseKind::InSine;
}
