#include "pch.h"
#include "CP_UISlider.h"
#include "GameObject.h"

CP_UISlider::CP_UISlider()
	: pUiRenderer(nullptr), startPos(0.0f), currentValue(1.0f), maxValue(1.0f), minValue(0.0f), sliderAllLength(100.0f)
{
}

void CP_UISlider::Init()
{
	pUiRenderer = GetGameObject().GetComponent<CP_UIRenderer>();
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

nlohmann::json CP_UISlider::Save()
{
	auto data = Component::Save();

	data["startPos"] = startPos;
	data["currentValue"] = currentValue;
	data["maxValue"] = maxValue;
	data["minValue"] = minValue;
	data["sliderLen"] = sliderAllLength;

	return data;
}

void CP_UISlider::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;

	Component::Load(_data);

	LoadJsonFloat("startPos", startPos, _data);
	LoadJsonFloat("currentValue", currentValue, _data);
	LoadJsonFloat("maxValue", maxValue, _data);
	LoadJsonFloat("minValue", minValue, _data);
	LoadJsonFloat("sliderLen", sliderAllLength, _data);

	ApplySlider();
}

void CP_UISlider::ApplySlider()
{
	using namespace DirectX::SimpleMath;

	Transform& transform = GetTransform();

	float curRatio = (currentValue - minValue) / (maxValue - minValue);
	float curLength = sliderAllLength * curRatio;
	Vector3 scale = transform.GetLocalScale();
	scale.x = curLength;
	transform.SetLocalScale(scale);

	Vector3 pos = transform.GetLocalPosition();
	pos.x = startPos + transform.GetLocalScale().x * 0.5f;
	transform.SetLocalPosition(pos);

	if (pUiRenderer)
		pUiRenderer->SetUV(Vector2::Zero, Vector2(curRatio, 1.0f));
}

void CP_UISlider::ImGuiDebug()
{
	if (ImGui::Button("Set Render"))
	{
		pUiRenderer = GetGameObject().GetComponent<CP_UIRenderer>();
	}

	if (ImGui::DragFloat("startPos", &startPos))
	{
		ApplySlider();
	}

	if (ImGui::DragFloat("length", &sliderAllLength))
	{
		ApplySlider();
	}

	float imFloat = currentValue;
	if (ImGui::SliderFloat("current", &imFloat, 0.0f, 1.0f))
	{
		SetCurrentRatio(imFloat);
	}
}
