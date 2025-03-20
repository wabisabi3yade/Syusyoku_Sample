#include "pch.h"
#include "CP_Gage.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	CP_Gage::CP_Gage() : 
		pFrontSlider(nullptr),
		pBackSlider(nullptr),
		currentValue(0.0f),
		maxValue(10.0f),
		minValue(0.0f)
	{
	}

	void CP_Gage::SetCurrentRatio(float _curRatio)
	{
		SetCurrentValue((maxValue - minValue) * _curRatio + minValue);
	}

	void CP_Gage::SetCurrentValue(float _setValue)
	{
#ifdef EDIT
		if (!GetIsSettingSlider()) return;
#endif // EDIT

		// 正面スライダーは反映
		pFrontSlider->SetCurrentRatio(std::clamp(_setValue, minValue, maxValue));
	}

	void CP_Gage::SetMaxValue(float _maxVal)
	{
#ifdef EDIT
		if (!GetIsSettingSlider()) return;
#endif // EDIT

		maxValue = std::max(_maxVal, minValue);

		// 両スライダーの最大値をセット
		pFrontSlider->SetMaxValue(maxValue);
		pBackSlider->SetMaxValue(maxValue);
	}

	void CP_Gage::SetMinValue(float _minVal)
	{
#ifdef EDIT
		if (!GetIsSettingSlider()) return;
#endif // EDIT

		minValue = std::min(_minVal, maxValue);

		// 両スライダーの最低値をセット
		pFrontSlider->SetMinValue(minValue);
		pBackSlider->SetMinValue(minValue);
	}

	json CP_Gage::Save()
	{
		json data = Component::Save();

		data["frontName"] = frontObjName;
		data["backName"] = backObjName;

		return data;
	}

	void CP_Gage::Load(const json& _data)
	{
		Component::Load(_data);

		LoadJsonString("frontName", frontObjName, _data);
		LoadJsonString("backName", backObjName, _data);
	}

	void CP_Gage::Start()
	{
		// スライダーを取得
		pFrontSlider = GetSlider(frontObjName);
		pBackSlider = GetSlider(backObjName);

		// 最大値と最小値をセット
		if (GetIsSettingSlider())
		{
			pFrontSlider->SetMaxValue(maxValue);
			pFrontSlider->SetMinValue(minValue);
			pBackSlider->SetMaxValue(maxValue);
			pBackSlider->SetMinValue(minValue);
		}
	}

	bool CP_Gage::GetIsSettingSlider() const
	{
		if (pFrontSlider == nullptr || pBackSlider == nullptr)
		{
			HASHI_DEBUG_LOG("スライダーがセットされていません");
			return false;
		}

		return true;
	}

	void CP_Gage::ImGuiDebug()
	{
#ifdef EDIT
		Component::ImGuiDebug();

		// 入力
		static char input[IM_INPUT_BUF];
		ImGui::InputText("Object Name", input, IM_INPUT_BUF);

		// 正面
		if (ImGui::Button("Set Front"))
			frontObjName = input;
		ImGui::SameLine();
		ImGui::Text(frontObjName.c_str());

		// 正面
		if (ImGui::Button("Set Back"))
			backObjName = input;
		ImGui::SameLine();
		ImGui::Text(backObjName.c_str());

#endif // EDIT
	}

	IUISlider* CP_Gage::GetSlider(const std::string& _objName)
	{
		// シーンオブジェクトを取得
		GameObject* pObj = 
			InSceneSystemManager::GetInstance()->
			GetSceneObjects().GetSceneObject(_objName);
		if (!pObj) return nullptr;

		// スライダーを取得
		IUISlider* pUISlider = pObj->GetComponent<IUISlider>();
		return pUISlider;
	}
}

