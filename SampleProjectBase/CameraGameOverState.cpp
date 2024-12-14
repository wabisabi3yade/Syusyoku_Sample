#include "pch.h"
#include "CameraGameOverState.h"

namespace DXSimp = HashiTaku::DXSimp;

CameraGameOverState::CameraGameOverState() :
	pTargetTransform(nullptr),
	horiDistance(3.0f), 
	heightDistance(3.0f)
{
}

void CameraGameOverState::OnStartBehavior()
{
	if (!pTargetTransform) return;

	// カメラの向きをターゲットに向ける
	GetCamera().GetTransform().LookAt(pTargetTransform->GetPosition());
}

void CameraGameOverState::UpdateBehavior()
{
	if (!pTargetTransform) return;

	DXSimp::Vector3 camPos = pTargetTransform->GetPosition();
	camPos += pTargetTransform->Forward() * horiDistance;
	camPos += Vec3::Up * heightDistance;
	SetBasePosition(camPos);

	Transform& camTrans = GetCamera().GetTransform();

	DXSimp::Vector3 worldOffset = camTrans.Up() * lookOffset.y;
	worldOffset += camTrans.Right() * lookOffset.x;

	camTrans.LookAt(pTargetTransform->GetPosition() + worldOffset);
}

void CameraGameOverState::SetTargetTransform(const Transform& _targetTransform)
{
	pTargetTransform = &_targetTransform;
}

nlohmann::json CameraGameOverState::Save()
{
	auto data = CameraMoveState_Base::Save();
	HashiTaku::SaveJsonVector2("lookOffset", lookOffset, data);
	data["hori"] = horiDistance;
	data["height"] = heightDistance;
	return data;
}

void CameraGameOverState::Load(const nlohmann::json& _data)
{
	CameraMoveState_Base::Load(_data);
	HashiTaku::LoadJsonVector2("lookOffset", lookOffset, _data);
	HashiTaku::LoadJsonFloat("hori", horiDistance, _data);
	HashiTaku::LoadJsonFloat("height", heightDistance, _data);

}

void CameraGameOverState::ImGuiDebug()
{
	ImGui::DragFloat2("LookOffset", &lookOffset.x, 0.01f);
	ImGui::DragFloat("Hori", &horiDistance, 0.01f);
	ImGui::DragFloat("Height", &heightDistance, 0.01f);
}
