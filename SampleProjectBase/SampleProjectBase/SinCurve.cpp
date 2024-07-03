#include "pch.h"
#include "SinCurve.h"

void SinCurve::Init()
{
	name = "SinCurve";

	speed = 180.0f;
	power = 1.0f;
	sinAngle = 0.0f;
}

void SinCurve::Update()
{

	Transform& transform = GetTransform();

	sinAngle += speed * MainApplication::DeltaTime();

	if (abs(sinAngle) > 360.0f)
		sinAngle = std::fmod(sinAngle, 360.0f);

	transform.position.y = power * sin(sinAngle * Mathf::degToRad);
}

void SinCurve::ImGuiSetting()
{
	ImGui::DragFloat("speed", &speed);
	ImGui::DragFloat("power", &power);
}
