#pragma once
#include "Component.h"

class SinCurve : public Component
{
	float speed;
	float power;
	float sinAngle;
public:
	using Component::Component;
	void Init() override;

	void Update() override;

	void ImGuiSetting()override;
};

