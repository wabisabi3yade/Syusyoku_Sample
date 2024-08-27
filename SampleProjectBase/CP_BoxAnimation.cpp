#include "pch.h"
#include "CP_BoxAnimation.h"

#include "Geometory.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

void CP_BoxAnimation::ImGuiSetting()
{
	if (ImGui::Button("Transition"))
		TransitionStart();

	if (ImGui::Button("progress"))
		isStop = false;

	ImGuiMethod::DragFloat3(vec, "Vec", 0.1f);
	ImGuiMethod::DragFloat3(requestPose, "request");

	ImGui::Text("last");
	ImGuiMethod::Text(lastQuat);

	ImGui::Text("second");
	ImGuiMethod::Text(secondQuat);
}

void CP_BoxAnimation::Update()
{
	if (!isTransition)
	{
		Vector3 a = GetTransform().GetLocalEularAngles();
		a += vec * MainApplication::DeltaTime();
		a = Vec3::Repeat(a, 360.0f);
		GetTransform().SetLocalEularAngles(a);

		secondQuat = std::move(lastQuat);
		secondTime = lastTime;
		lastQuat = GetTransform().GetLocalRotation();
		lastTime = MainApplication::DeltaTime();
	}
	else
	{
		Transition();
	}
}

void CP_BoxAnimation::Draw()
{
	Geometory::SetPosition(GetTransform().GetLocalPosition());
	Geometory::SetRotation(GetTransform().GetLocalEularAngles());
	Geometory::SetColor(Color(0, 1, 0, 1));
	Geometory::DrawCube();

	Geometory::SetPosition(GetTransform().GetLocalPosition() + Vec3::Right * dis);
	Geometory::SetRotation(requestPose);
	Geometory::SetColor(Color(1, 1, 0, 1));
	Geometory::DrawCube();
}

nlohmann::json CP_BoxAnimation::Save()
{
	auto data = Component::Save();

	SaveJsonVector3("Vec", vec, data);
	SaveJsonVector3("Request", requestPose, data);
	return data;
}

void CP_BoxAnimation::Load(const nlohmann::json& _data)
{
	Component::Load(_data);
	LoadJsonVector3("Vec", vec, _data);
	LoadJsonVector3("Request", requestPose, _data);
}

void CP_BoxAnimation::Transition()
{
	/*if (isStop) return;*/

	culElapsedBlend += MainApplication::DeltaTime();

	if (culElapsedBlend > blendTime)
	{
		isTransition = false;
		return;
	}

	float blendValue = CalcBlend(transition, culElapsedBlend);

	Quaternion q = Quaternion::CreateFromAxisAngle(transition.axis, blendValue);

	GetTransform().SetLocalRotation(Quat::Multiply(transQuat, q));

	isStop = true;
}

void CP_BoxAnimation::TransitionStart()
{
	isTransition = true;
	culElapsedBlend = 0.0f;
	transQuat = GetTransform().GetLocalRotation();

	assert(lastTime > 0.0f);

	Quaternion invRequestPose;
	Quaternion request = Quaternion::CreateFromYawPitchRoll(requestPose.y, requestPose.x, requestPose.z);
	request.Inverse(invRequestPose);

	Quaternion q0 = Quat::Multiply(lastQuat, invRequestPose);

	Quat::ToAxisAngle(q0, transition.axis, transition.x0);
	transition.axis.Normalize();
	transition.x0 = Mathf::Repeat(transition.x0 + Mathf::PI, Mathf::PI * 2.0f) - Mathf::PI;

	Quaternion qn1 = Quat::Multiply(secondQuat, invRequestPose);

	float xn1 = Mathf::PI;

	if (abs(qn1.w) > Mathf::epsilon)
	{
		Vector3 q_xyz = Vector3(qn1.x, qn1.y, qn1.z);

		xn1 = 2.0f * atan(q_xyz.Dot(transition.axis) / qn1.w);
		xn1 = Mathf::Repeat(xn1 + Mathf::PI, Mathf::PI * 2) - Mathf::PI;
	}

	float deltaAngle = transition.x0 - xn1;
	deltaAngle = Mathf::Repeat(deltaAngle + Mathf::PI, Mathf::PI * 2.0f) - Mathf::PI;

	transition.v0 = deltaAngle / lastTime;

	// Calculate

	if (transition.x0 <= Mathf::epsilon)
	{
		transition.t1 = 0.0f;
		transition.a0 = 0.0f;
		transition.A = 0.0f;
		transition.B = 0.0f;
		transition.C = 0.0f;
	}
	else
	{
		if (transition.v0 != 0.0f && (transition.x0 / transition.v0) < 0.0f)
		{
			transition.t1 = std::min(blendTime, -5.0f * transition.x0 / transition.v0);
		}
		else
		{
			transition.t1 = blendTime;
		}

		// ŽžŠÔ‚Ì—Ýæ‚ðŒvŽZ
		float t1_2 = transition.t1 + transition.t1;
		float t1_3 = t1_2 + transition.t1;
		float t1_4 = t1_3 + transition.t1;

		/*transition.a0 = (-8.0f * transition.v0 * transition.t1 - 20.0f * transition.x0) / t1_2;
		transition.A = -(transition.a0 * t1_2 + 6.0f * transition.v0 * transition.t1 + 12.0f * transition.x0) / (2.0f * t1_4 * transition.t1);
		transition.B = (3.0f * transition.a0 * t1_2 + 16.0f * transition.v0 * transition.t1 + 30.0f * transition.x0) / (2.0f * t1_4);
		transition.C = -(3.0f * transition.a0 * t1_2 + 12.0f * transition.v0 * transition.t1 + 20.0f * transition.x0) / (2.0f * t1_3);*/

		transition.a0 = ((-8.f * transition.v0 * transition.t1) + (-20.f * transition.x0)) / pow(transition.t1, 2.f);

		transition.A = -((1.f * transition.a0 * transition.t1 * transition.t1) + (6.f * transition.v0 * transition.t1) + (12.f * transition.x0)) / (2.f * pow(transition.t1, 5.f));
		transition.B = ((3.f * transition.a0 * transition.t1 * transition.t1) + (16.f * transition.v0 * transition.t1) + (30.f * transition.x0)) / (2.f * pow(transition.t1, 4.f));
		transition.C = -((3.f * transition.a0 * transition.t1 * transition.t1) + (12.f * transition.v0 * transition.t1) + (20.f * transition.x0)) / (2.f * pow(transition.t1, 3.f));
	}
}

float CP_BoxAnimation::CalcBlend(TransitionBase trans, float blendTime)
{
	float t_1 = std::min(blendTime, trans.t1);
	float t_2 = t_1 * t_1;
	float t_3 = t_1 * t_2;
	float t_4 = t_1 * t_3;
	float t_5 = t_1 * t_4;

	return (trans.A * t_5) + (trans.B * t_4) + (trans.C * t_3) + (0.5f * trans.a0 * t_2) + (trans.v0 * t_1)/* + (trans.x0)*/;
}
