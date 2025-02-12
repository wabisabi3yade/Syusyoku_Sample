#include "pch.h"
#include "CP_EaseTest.h"

#include "Geometory.h"

namespace HashiTaku
{
	using enum EaseKind;

	CP_EaseTest::CP_EaseTest() : nowEase(InSine)
	{
	}

	void CP_EaseTest::Start()
	{
	}

	void CP_EaseTest::Update()
	{
		elapsedTime += vec * DeltaTime();

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

		float p = Easing::EaseValue(elapsedTime / time, nowEase);

		Transform& transform = GetTransform();
		DXSimp::Vector3 pos = transform.GetPosition();
		pos.x = p * magnitude - magnitude * 0.5f;
		transform.SetPosition(pos);
	}

	void CP_EaseTest::Draw()
	{
		Geometory::SetPosition(GetTransform().GetPosition());
		Geometory::SetScale(DXSimp::Vector3::One * 0.4f);
		Geometory::DrawSphere();

		DXSimp::Vector3 p = GetTransform().GetPosition();
		p.x = -magnitude * 0.5f;
		// �n�_
		Geometory::SetPosition(p);
		Geometory::SetScale(DXSimp::Vector3::One * 0.4f);
		Geometory::SetColor(DXSimp::Color(1, 0, 0, 1));
		Geometory::DrawSphere();

		p = GetTransform().GetPosition();
		p.x = magnitude * 0.5f;
		// �n�_
		Geometory::SetPosition(p);
		Geometory::SetScale(DXSimp::Vector3::One * 0.4f);
		Geometory::SetColor(DXSimp::Color(0, 0, 1, 1));
		Geometory::DrawSphere();
	}

	void CP_EaseTest::ImGuiDebug()
	{
		ImGui::DragFloat("magnitude", &magnitude);
		ImGui::DragFloat("time", &time, 0.1f);
		Easing::ImGuiSelect(nowEase);
	}

	json CP_EaseTest::Save()
	{
		auto data = Component::Save();

		data["magnitude"] = magnitude;
		data["time"] = time;
		data["nowEase"] = nowEase;

		return data;
	}

	void CP_EaseTest::Load(const json& _data)
	{
		Component::Load(_data);

		LoadJsonFloat("magnitude", magnitude, _data);
		LoadJsonFloat("time", time, _data);
		LoadJsonEnum<EaseKind>("nowEase", nowEase, _data);
	}
}