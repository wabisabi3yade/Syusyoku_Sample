#include "pch.h"
#include "ANS_ChangeParameter.h"
#include "AnimationParameters.h"

using namespace HashiTaku::AnimParam;

ANS_ChangeParameter::ANS_ChangeParameter()
	: AnimationNotifyState(NotifyType::ANS_ChangeParameter), pAnimationParameters(nullptr), parameterType(TypeKind::None)
{
	for (short s_i = 0; s_i < SECTION_CNT; s_i++)
	{
		isUsing[s_i] = false;
		setParameter[s_i] = false;
	}
}

void ANS_ChangeParameter::OnTerminal()
{
	if (!IsCanSet(SectionType::Terminal)) return;

	for (auto& paramName : changeParamNames)
		pAnimationParameters->SetBool(paramName, setParameter[CastShort(SectionType::Terminal)]);
}

void ANS_ChangeParameter::SetAnimationParameters(AnimationParameters& _animatiionParameters)
{
	pAnimationParameters = &_animatiionParameters;
}

std::unique_ptr<AnimationNotify_Base> ANS_ChangeParameter::Clone()
{
	return std::make_unique<ANS_ChangeParameter>(*this);
}

nlohmann::json ANS_ChangeParameter::Save()
{
	auto data = AnimationNotifyState::Save();

	data["parameterNames"] = changeParamNames;

	// セクションごとのセーブ
	for (auto s_i = 0; s_i < SECTION_CNT; s_i++)
	{
		data["isUse"][s_i] = isUsing[s_i];
		data["setParameter"][s_i] = setParameter[s_i];
	}

	return data;
}

void ANS_ChangeParameter::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;

	AnimationNotifyState::Load(_data);

	if (HashiTaku::IsJsonContains(_data, "parameterNames"))
		changeParamNames = _data["parameterNames"].get<std::vector<std::string>>();

	// セクションごとのセーブ
	for (auto s_i = 0; s_i < SECTION_CNT; s_i++)
	{
		nlohmann::json useData;
		nlohmann::json setParamData;

		// 使用フラグ
		if (LoadJsonDataArray("isUse", useData, _data))
		{
			isUsing[s_i] = useData[s_i];
		}

		// パラメータ
		if (LoadJsonDataArray("setParameter", setParamData, _data))
		{
			setParameter[s_i] = setParamData[s_i];
		}
	}
}

void ANS_ChangeParameter::Tick()
{
	if (!IsCanSet(SectionType::Tick)) return;

	for (auto& paramName : changeParamNames)
		pAnimationParameters->SetBool(paramName, setParameter[CastShort(SectionType::Tick)]);
}

void ANS_ChangeParameter::End()
{
	if (!IsCanSet(SectionType::End)) return;

	for (auto& paramName : changeParamNames)
		pAnimationParameters->SetBool(paramName, setParameter[CastShort(SectionType::End)]);
}

void ANS_ChangeParameter::OnInitialize()
{
	if (!IsCanSet(SectionType::Init)) return;

	for (auto& paramName : changeParamNames)
		pAnimationParameters->SetBool(paramName, setParameter[CastShort(SectionType::Init)]);
}

bool ANS_ChangeParameter::IsCanSet(SectionType _type)
{
	if (!isUsing[CastShort(_type)]) return false;

	return true;
}

short ANS_ChangeParameter::CastShort(SectionType _type)
{
	return static_cast<short>(_type);
}

void ANS_ChangeParameter::ImGuiDebug()
{
	// 各行で横をそろえるためのスペース幅s
	const float TAB_SPACE_USE(140);
	const float TAB_SPACE_SETPARAM(240);

	AnimationNotifyState::ImGuiDebug();

	// 今追加されているパラメータ
	auto paramItr = changeParamNames.begin();
	for (; paramItr != changeParamNames.end();)
	{
		ImGui::Text((*paramItr).c_str());
		ImGui::SameLine();
		// 削除
		if (ImGui::Button("X")) paramItr = changeParamNames.erase(paramItr);
		else ++paramItr;
	}

	// パラメータ名を追加
	std::vector<const std::string*> parameterNames;
	pAnimationParameters->GetNameList(parameterNames);	// パラメータ名一覧取得
	static std::string getParamStr;
	ImGuiMethod::ComboBox("Parameter", getParamStr, parameterNames);
	if (ImGui::Button("+"))
	{
		// bool型では
		if (pAnimationParameters->GetParameterType(getParamStr) == TypeKind::Bool)
		{
			// 重複していないなら追加
			auto findItr = std::find(changeParamNames.begin(), changeParamNames.end(), getParamStr);
			if (findItr == changeParamNames.end()) changeParamNames.push_back(getParamStr);
		}
		else	// boolでないと
		{
			HASHI_DEBUG_LOG("bool型の変数をセットしてください");
		}
	}

	// 各セクションの編集
	std::vector<std::string> typeName =
	{
		"Init",
		"Tick",
		"End",
		"Terminal"
	};

	for (short s_i = 0; s_i < SECTION_CNT; s_i++)
	{
		ImGui::PushID(s_i);

		ImGui::Text(typeName[s_i].c_str());

		ImGui::SameLine(TAB_SPACE_USE);

		// 使用フラグを変更
		ImGui::Checkbox("isUse", &isUsing[s_i]);

		ImGui::SameLine(TAB_SPACE_SETPARAM);

		// パラメータセットするフラグ
		ImGui::Checkbox("setBool", &setParameter[s_i]);

		ImGui::PopID();
	}
}
