#include "pch.h"
#include "CP_UISlider.h"
#include "GameObject.h"

namespace HashiTaku
{
	CP_UISlider::CP_UISlider()
		: currentValue(1.0f), maxValue(1.0f), minValue(0.0f)
	{
	}

	void CP_UISlider::Init()
	{
		CP_UIRenderer::Init();
		sliderScale = { 100.0f, 100.0f };
	}

	void CP_UISlider::OnChangeScale()
	{
		ApplySlider();
	}

	void CP_UISlider::OnChangePosition()
	{
		ApplySlider();
	}

	void CP_UISlider::OnChangeRotation()
	{
		ApplySlider();
	}

	void CP_UISlider::SetCurrentRatio(float _curRatio)
	{
		// 範囲制限
		_curRatio = std::clamp<float>(_curRatio, 0.0f, 1.0f);

		// 割合から現在値をセット
		float length = maxValue - minValue;
		float curLength = length * _curRatio;
		currentValue = minValue + curLength;

		ApplySlider();
	}

	void CP_UISlider::SetCurrentValue(float _curVal)
	{
		currentValue = _curVal;
		ApplySlider();
	}

	void CP_UISlider::SetMaxValue(float _maxVal)
	{
		maxValue = _maxVal;
		ApplySlider();
	}

	void CP_UISlider::SetMinValue(float _minVal)
	{
		minValue = _minVal;
		ApplySlider();
	}

	json CP_UISlider::Save()
	{
		auto data = CP_UIRenderer::Save();

		data["currentValue"] = currentValue;
		data["maxValue"] = maxValue;
		data["minValue"] = minValue;

		return data;
	}

	void CP_UISlider::Load(const json& _data)
	{
		using namespace HashiTaku;

		CP_UIRenderer::Load(_data);

		LoadJsonFloat("currentValue", currentValue, _data);
		LoadJsonFloat("maxValue", maxValue, _data);
		LoadJsonFloat("minValue", minValue, _data);

		ApplySlider();
	}

	void CP_UISlider::ApplySlider()
	{
		using namespace DXSimp;

		Transform& transform = GetTransform();

		float curRatio = (currentValue - minValue) / (maxValue - minValue);

		// スライダーのスケール
		sliderScale.x = curRatio * transform.GetScale().x;
		sliderScale.y = transform.GetScale().y;

		// スライダー画像の中心座標を求める
		sliderCenterPos = transform.GetPosition();
		sliderCenterPos.x += sliderScale.x * 0.5f;

		SetUV(Vector2::Zero, Vector2(curRatio, 1.0f));

		ReCreatePolygon();	// ポリゴン再生成
	}

	void CP_UISlider::ReCreatePolygon()
	{
		Transform& transform = GetTransform();

		// トランスフォームの座標ではなく、スライダー画像の中心座標を渡す
		pPolygon->MakePolygon(sliderCenterPos, sliderScale, transform.GetEularAngles());
	}

	void CP_UISlider::ImGuiDebug()
	{
		CP_UIRenderer::ImGuiDebug();

		float imFloat = currentValue;
		if (ImGui::SliderFloat("current", &imFloat, 0.0f, 1.0f))
		{
			SetCurrentRatio(imFloat);
		}
	}
}