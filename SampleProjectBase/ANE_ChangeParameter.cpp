#include "pch.h"
#include "ANE_ChangeParameter.h"
#include "AnimationParameters.h"

namespace Anim = HashiTaku::AnimParam;

ANE_ChangeParameter::ANE_ChangeParameter()
	: AnimationNotifyEvent(NotifyType::ANE_ChangeParameter),
	pAnimationParameters(nullptr), setParam(false)
{
}

void ANE_ChangeParameter::SetAnimationParameters(AnimationParameters& _animatiionParameters)
{
	pAnimationParameters = &_animatiionParameters;
}

std::unique_ptr<AnimationNotify_Base> ANE_ChangeParameter::Clone()
{
	return std::make_unique<ANE_ChangeParameter>(*this);
}

nlohmann::json ANE_ChangeParameter::Save()
{
	auto data = AnimationNotifyEvent::Save();

	data["paramName"] = changeParamName;
	data["setParam"] = setParam;

	return data;
}

void ANE_ChangeParameter::Load(const nlohmann::json& _data)
{
	AnimationNotifyEvent::Load(_data);

	HashiTaku::LoadJsonString("paramName", changeParamName, _data);
	HashiTaku::LoadJsonBoolean("setParam", setParam, _data);
}

void ANE_ChangeParameter::OnEvent()
{
	pAnimationParameters->SetBool(changeParamName, setParam);
}

void ANE_ChangeParameter::OnInitialize()
{
}

void ANE_ChangeParameter::OnTerminal()
{
	pAnimationParameters->SetBool(changeParamName, !setParam);
}

void ANE_ChangeParameter::ImGuiDebug()
{
	AnimationNotifyEvent::ImGuiDebug();

	// パラメータ名を変更する
	std::vector<const std::string*> parameterNames;
	pAnimationParameters->GetNameList(parameterNames);	// パラメータ名一覧取得
	std::string string = changeParamName;
	if (ImGuiMethod::ComboBox("Parameter", string, parameterNames))
	{
		// bool型では
		if (pAnimationParameters->GetParameterType(string) == 
			Anim::TypeKind::Bool)
		{
			changeParamName = string;
		}
		else	// boolでないと
		{
			HASHI_DEBUG_LOG("bool型の変数をセットしてください");
		}
	}

	// パラメータセットするフラグ
	ImGui::Checkbox("setBool", &setParam);
}
