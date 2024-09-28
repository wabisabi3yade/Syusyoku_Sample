#include "pch.h"
#include "AnimationParameters.h"

using namespace HashiTaku::AnimParam;

void AnimationParameters::SetBool(const std::string& _paramName, bool _isBool)
{
#ifdef EDIT
	if(!CheckBool(_paramName)) return;
#endif // EDIT

	animParameters[_paramName] = _isBool;
}

void AnimationParameters::SetInt(const std::string& _paramName, int _intVal)
{
#ifdef EDIT
	if (CheckInt(_paramName)) return;
#endif // EDIT

	animParameters[_paramName] = _intVal;
}

void AnimationParameters::SetFloat(const std::string& _paramName, float _floatVal)
{
#ifdef EDIT
	if (CheckFloat(_paramName)) return;
#endif // EDIT

	animParameters[_paramName] = _floatVal;
}

bool AnimationParameters::GetBool(const std::string& _paramName)
{
#ifdef EDIT
	if (!CheckBool(_paramName)) return false;
#endif // EDIT

	return std::get<bool>(animParameters[_paramName]);
}

int AnimationParameters::GetInt(const std::string& _paramName)
{
#ifdef EDIT
	if (!CheckInt(_paramName)) return 0;
#endif // EDIT

	return std::get<int>(animParameters[_paramName]);
}

float AnimationParameters::GetFloat(const std::string& _paramName)
{
#ifdef EDIT
	if (!CheckFloat(_paramName)) return 0.0f;
#endif // EDIT

	return std::get<float>(animParameters[_paramName]);
}

void AnimationParameters::GetNameList(std::vector<const std::string*>& _nameList)
{
	for (auto& param : animParameters)
	{
		_nameList.push_back(&param.first);
	}
}

const conditionValType* AnimationParameters::GetValueAddress(const std::string& _name) const
{
	if(!animParameters.contains(_name)) return nullptr;

	auto itr = animParameters.find(_name);
	return &itr->second;
}

const std::string* AnimationParameters::GetNameAddress(const std::string& _name) const
{
	if (!animParameters.contains(_name)) return nullptr;

	auto itr = animParameters.find(_name);
	return &itr->first;
}

const std::string* AnimationParameters::GetBeginNameAddres() const
{
	if (animParameters.empty()) return nullptr;

	return &animParameters.begin()->first;
}

const conditionValType* AnimationParameters::GetBeginValueAddres() const
{
	if (animParameters.empty()) return nullptr;

	return &animParameters.begin()->second;
}

u_int AnimationParameters::GetParameterCnt() const
{
	return static_cast<u_int>(animParameters.size());
}

nlohmann::json AnimationParameters::Save()
{
	nlohmann::json parameterListData;

	for (auto& param : animParameters)
	{
		nlohmann::json parameterData;
		parameterData["name"] = param.first;
		
		TypeKind type = GetType(param.second);
		parameterData["type"] = type;

		const std::string element = "value";
		switch (type)
		{
		case TypeKind::Bool:
			parameterData[element] = std::get<bool>(param.second);
			break;
		case TypeKind::Int:
			parameterData[element] = std::get<int>(param.second);
			break;
		case TypeKind::Float:
			parameterData[element] = std::get<float>(param.second);
			break;
		}

		parameterListData.push_back(parameterData);
	}

	return parameterListData;
}

void AnimationParameters::Load(const nlohmann::json& _data)
{
	for (auto& paramData : _data)
	{
		std::string name;
		if (!HashiTaku::LoadJsonString("name", name, paramData)) continue;

		TypeKind type;
		if (!HashiTaku::LoadJsonEnum<TypeKind>("type", type, paramData)) continue;

		conditionValType value;
		const std::string element = "value";
		switch (type)
		{
		case TypeKind::Bool:
		{
			bool isBool = false; HashiTaku::LoadJsonBoolean(element, isBool, paramData);
			value = isBool;
		}
			break;

		case TypeKind::Int:
		{
			int intVal = 0;  HashiTaku::LoadJsonInteger(element, intVal, paramData);
			value = intVal;
		}
			break;

		case TypeKind::Float:
		{
			float floatVal = 0.0f;  HashiTaku::LoadJsonFloat(element, floatVal, paramData);
			value = floatVal;
		}
			break;
		}

		animParameters[name] = value;
	}
}

void AnimationParameters::NotDuplicateParamName(std::string& _paramName)
{
	u_int loopCnt = 1;
	std::string startName = _paramName;

	while (animParameters.contains(_paramName))	// 同じ名前がある限りループ
	{
		_paramName = startName + std::to_string(loopCnt);
		loopCnt++;
	}
}

bool AnimationParameters::CheckBool(const std::string& _name)
{
	// 不正な値を参照していないかチェック
	if (!animParameters.contains(_name))
	{
		HASHI_DEBUG_LOG(_name + "が見つかりませんでした");
		return false;
	}
	conditionValType getVal = animParameters[_name];
	if (!std::holds_alternative<bool>(getVal))
	{
		HASHI_DEBUG_LOG(_name + "はbool型ではありません");
		return false;
	}

	return true;
}

bool AnimationParameters::CheckInt(const std::string& _name)
{
	// 不正な値を参照していないかチェック
	if (!animParameters.contains(_name))
	{
		HASHI_DEBUG_LOG(_name + "が見つかりませんでした");
		return false;
	}
	conditionValType getVal = animParameters[_name];
	if (!std::holds_alternative<int>(getVal))
	{
		HASHI_DEBUG_LOG(_name + "はint型ではありません");
		return false;
	}

	return true;
}

bool AnimationParameters::CheckFloat(const std::string& _name)
{
	// 不正な値を参照していないかチェック
	if (!animParameters.contains(_name))
	{
		HASHI_DEBUG_LOG(_name + "が見つかりませんでした");
		return false;
	}
	conditionValType getVal = animParameters[_name];
	if (!std::holds_alternative<float>(getVal))
	{
		HASHI_DEBUG_LOG(_name + "はfloat型ではありません");
		return false;
	}
	return true;
}

TypeKind AnimationParameters::GetType(const conditionValType& _parameter)
{
	TypeKind retType = TypeKind::Bool;

	if (std::holds_alternative<int>(_parameter))
		retType = TypeKind::Int;
	else if (std::holds_alternative<float>(_parameter))
		retType = TypeKind::Float;

	return retType;
}

void AnimationParameters::ImGuiSetting()
{
	ImGui::Text("Parameter");
	ImGuiAddParam();
	ImGuiDisplay();
}

void AnimationParameters::ImGuiAddParam()
{
#ifdef EDIT
	std::vector<std::string> str =
	{
		"Bool",
		"Int",
		"Float"
	};
	static u_int selectId = 0;

	ImGuiMethod::ComboBox("##ParamCombo", selectId, str);
	ImGui::SameLine();
	if (ImGui::Button("+"))
	{
		TypeKind paramType = static_cast<TypeKind>(selectId);

		switch (paramType)
		{
		case TypeKind::Bool:
			AddParameter<bool>();
			break;
		case TypeKind::Int:
			AddParameter<int>();
			break;
		case TypeKind::Float:
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
	// 名前表示(変更)
	int idx = 0;
	auto itr = animParameters.begin();
	while (itr != animParameters.end())
	{
		ImGuiMethod::LineSpaceSmall();
		bool isDelete = false;
		// 変数名を変更
		char buf[IM_INPUT_BUF];
		strncpy_s(buf, itr->first.c_str(), sizeof(buf));
		ImGui::PushID(itr->first.c_str());
		if (ImGui::InputText("##param", buf, IM_INPUT_BUF, ImGuiInputTextFlags_EnterReturnsTrue))	// 変更されたら
		{
			// 元の名前を削除して、新しく追加する
			isDelete = true;
			std::string newKey = buf;
			conditionValType newParam = itr->second;
			NotDuplicateParamName(newKey);

			animParameters[newKey] = newParam;	// 追加
		}
		ImGui::SameLine();
		// パラメータ操作
		if (bool* pBool = std::get_if<bool>(&itr->second))	// bool
			ImGui::Checkbox("##bool", pBool);
		else if (int* pInt = std::get_if<int>(&itr->second))// int
		{
			ImGuiMethod::PushItemWidth();
			ImGui::DragInt("##int", pInt);
			ImGui::PopItemWidth();
		}
		else if (float* pFloat = std::get_if<float>(&itr->second))	// float
		{
			ImGuiMethod::PushItemWidth();
			ImGui::DragFloat("##float", pFloat);
			ImGui::PopItemWidth();
		}
		ImGui::SameLine();
		if (ImGui::Button("X"))	// 削除
			isDelete = true;

		ImGui::PopID();

		idx++;
		if (isDelete)
		{
			itr = animParameters.erase(itr);
		}
		else
			itr++;
	}

#endif // EDIT
}
