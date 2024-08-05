#include "pch.h"
#include "CP_EaseTest.h"

#include "Geometory.h"

using namespace HashiTaku;
using namespace DirectX::SimpleMath;

using enum EaseKind;
using enum EaseFuncKind;

CP_EaseTest::CP_EaseTest() : nowEase({ Sine, In })
{
	name = "EaseTest";
	Easing::GetEase(easeFunc, nowEase);
}

void CP_EaseTest::Init()
{
	GetTransform().position.z = 5.0f;
	GetTransform().position.y = 2.0f;
}

void CP_EaseTest::Update()
{
	elapsedTime += vec * MainApplication::DeltaTime();

	if (elapsedTime > time)
	{
		vec = -1;
		elapsedTime = time;
	}
		
	else if (elapsedTime < 0.0f)
	{
		vec = 1;
		elapsedTime = 0.0f;
	}
	float p = easeFunc(elapsedTime / time);
	GetTransform().position.x = p * magnitude - magnitude * 0.5f;
}

void CP_EaseTest::Draw()
{
	Geometory::SetPosition(GetTransform().position);
	Geometory::SetScale(Vector3::One * 0.4f);
	Geometory::DrawSphere();

	Vector3 p = GetTransform().position;
	p.x = -magnitude * 0.5f;
	// 始点
	Geometory::SetPosition(p);
	Geometory::SetScale(Vector3::One * 0.4f);
	Geometory::SetColor(Color(1, 0, 0, 1));
	Geometory::DrawSphere();

	p = GetTransform().position;
	p.x = magnitude * 0.5f;
	// 始点
	Geometory::SetPosition(p);
	Geometory::SetScale(Vector3::One * 0.4f);
	Geometory::SetColor(Color(0, 0, 1, 1));
	Geometory::DrawSphere();
}

void CP_EaseTest::ImGuiSetting()
{
	ImGui::DragFloat("magnitude", &magnitude);
	ImGui::DragFloat("time", &time, 0.1f);	
	Easing::ImGuiSelect(easeFunc, nowEase);
}
