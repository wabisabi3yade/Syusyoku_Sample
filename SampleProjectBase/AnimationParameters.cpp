#include "pch.h"
#include "AnimationParameters.h"

nlohmann::json AnimationParameters::Save()
{
	return nlohmann::json();
}

void AnimationParameters::Load(const nlohmann::json& _data)
{
}

void AnimationParameters::NotDuplicateParamName(std::string& _paramName)
{
	u_int loopCnt = 1;
	std::string startName = _paramName;

	while (animParameters.contains(_paramName))	// 同じ名前がある限りループ
	{
		_paramName = startName + std::to_string(loopCnt);
	}
}

void AnimationParameters::ImGuiSetting()
{
	ImGuiAddParam();
	ImGuiDisplay();
}

void AnimationParameters::ImGuiAddParam()
{
#ifdef EDIT
	// コンボボックスで作成する
	enum class ParamType
	{
		Bool,
		Int,
		Float
	};
	std::vector<std::string> str =
	{
		"Bool",
		"Int",
		"Float"
	};
	static u_int selectId = 0;

	ImGuiMethod::PushRandID();
	ImGuiMethod::ComboBox("", selectId, str);
	ImGui::PopID();
	ImGui::SameLine();
	if (ImGui::Button("+"))
	{
		ParamType paramType = static_cast<ParamType>(selectId);

		switch (paramType)
		{
		case ParamType::Bool:
			AddParameter<bool>();
			break;
		case ParamType::Int:
			AddParameter<int>();
			break;
		case ParamType::Float:
			AddParameter<float>();
			break;
		default:
			assert(!"不正なパラメータタイプ");
			break;
		}
	}
#endif // EDIT
}

void AnimationParameters::ImGuiDisplay()
{
#ifdef EDIT
	
	
	auto itr = animParameters.begin();

	while(itr != animParameters.end())
	{
		bool isDelete = false;

		// 変数名を変更
		char buf[IM_INPUT_BUF];
		strncpy(buf, parameter.first.c_str(), sizeof(buf));
		ImGuiMethod::PushRandID();
		if (ImGui::InputText("", buf, IM_INPUT_BUF))
		{

		}
		ImGui::PopID();

		if (isDelete)
		{

		}
		else
			itr++;
	}
	



#endif // EDIT
}
