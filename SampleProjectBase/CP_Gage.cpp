#include "pch.h"
#include "CP_Gage.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	CP_Gage::CP_Gage() : 
		pFrontSlider(nullptr),
		pBackSlider(nullptr),
		backMoveEase(EaseKind::Linear),
		backState(BackState::NotUpdate),
		currentValue(0.0f),
		maxValue(10.0f),
		minValue(0.0f),
		backValueOnDecade(0.0f),
		backMoveTime(1.0f),
		backMoveElapsedTime(0.0f),
		backWaitTime(1.0f),
		decadeElapsedTime(0.0f)
	{
	}

	void CP_Gage::SetCurrentRatio(float _curRatio)
	{
		SetCurrentValue((maxValue - minValue) * _curRatio + minValue);
	}

	void CP_Gage::SetCurrentValue(float _setValue)
	{
		float prevCurValue = currentValue;
		currentValue = _setValue;
		
		if (!GetIsSettingSlider()) return;

		// 正面スライダーは反映
		pFrontSlider->SetCurrentValue(std::clamp(_setValue, minValue, maxValue));
		// 変更前より値が減ったら
		if (currentValue < prevCurValue)
		{
			// 後ろスライダーを待機状態にする
			backState = BackState::Wait;
			decadeElapsedTime = 0.0f;
			backValueOnDecade = prevCurValue;
			pBackSlider->SetCurrentValue(backValueOnDecade);
		}
		else
		{
			// 増えた場合、正面スライダーの値と同期させる
			pBackSlider->SetCurrentValue(currentValue);
		}
	}

	void CP_Gage::SetMaxValue(float _maxVal)
	{
		maxValue = std::max(_maxVal, minValue);

		if (!GetIsSettingSlider()) return;

		// 両スライダーの最大値をセット
		pFrontSlider->SetMaxValue(maxValue);
		pBackSlider->SetMaxValue(maxValue);
	}

	void CP_Gage::SetMinValue(float _minVal)
	{
		minValue = std::min(_minVal, maxValue);

		if (!GetIsSettingSlider()) return;

		// 両スライダーの最低値をセット
		pFrontSlider->SetMinValue(minValue);
		pBackSlider->SetMinValue(minValue);
	}

	float CP_Gage::GetCurentValue() const
	{
		return currentValue;
	}

	json CP_Gage::Save()
	{
		json data = Component::Save();

		data["frontName"] = frontObjName;
		data["backName"] = backObjName;
		data["backMoveTime"] = backMoveTime;
		data["backWaitTime"] = backWaitTime;
		data["backMoveEase"] = backMoveEase;

		return data;
	}

	void CP_Gage::Load(const json& _data)
	{
		Component::Load(_data);

		LoadJsonString("frontName", frontObjName, _data);
		LoadJsonString("backName", backObjName, _data);
		LoadJsonFloat("backMoveTime", backMoveTime, _data);
		LoadJsonFloat("backWaitTime", backWaitTime, _data);
		LoadJsonEnum<EaseKind>("backMoveEase", backMoveEase, _data);
	}

	void CP_Gage::Start()
	{
		// スライダーを取得
		pFrontSlider = GetSlider(frontObjName);
		pBackSlider = GetSlider(backObjName);

		// 最大値と最小値をセット
		if (GetIsSettingSlider())
		{
			SetMaxValue(maxValue);
			SetMinValue(minValue);
			SetCurrentValue(currentValue);
		}
	}

	void CP_Gage::Update()
	{
		// 後ろスライダーの更新処理
		BackSliderUpdate();
	}

	void CP_Gage::BackSliderUpdate()
	{
#ifdef EDIT
		if (!GetIsSettingSlider()) return;
#endif // EDIT

		switch (backState)
		{
		case BackState::Wait:	// 待機
		{
			decadeElapsedTime += DeltaTime();

			// 経過時間が待機時間超えたら
			if (decadeElapsedTime > backWaitTime)
			{
				backMoveElapsedTime = 0.0f;
				backState = BackState::Move;
			}
		}
			break;

		case BackState::Move:	// 移動
		{
			backMoveElapsedTime += DeltaTime();

			// 後ろスライダーの値を減らす
			float backValue = std::lerp(backValueOnDecade,
				currentValue,
				Easing::EaseValue(backMoveElapsedTime / backMoveTime, backMoveEase));

			// 後ろスライダーに反映
			pBackSlider->SetCurrentValue(backValue);

			// 移動を終えたら更新終了
			if (backMoveElapsedTime > backMoveTime)
				backState = BackState::NotUpdate;
		}
			break;

		default:
			break;
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

		ImGui::DragFloat("Back MoveTime", &backMoveTime, 0.1f, 0.0f, 1000.0f);
		Easing::ImGuiSelect(backMoveEase, "Back MoveEase");
		ImGui::DragFloat("Back WaitTime", &backWaitTime, 0.1f, 0.0f, 1000.0f);

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

