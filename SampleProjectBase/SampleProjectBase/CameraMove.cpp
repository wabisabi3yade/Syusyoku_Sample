#include "pch.h"
#include "CameraMove.h"
#include "GameObject.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;
void CameraMove::UpdatePosition()
{
}

void CameraMove::UpdateLook()
{
	// �����_�̖ڕW���W
	Vector3 lookTarget = pPlayer->transform.position + Vector3::Up * lookOffsetY;

	// ���`��ԂŌ��݂̒����_��ڕW���W�ɋ߂Â���
	lookPos = Vector3::Lerp(lookPos, lookTarget, lookSpeed * MainApplication::DeltaTime());

	/*Transform& t = GetTransform();
	t.LookAt(lookPos);*/
}

void CameraMove::Init()
{
	name = "CameraMove";

	camera = static_cast<Camera*>(gameObject);	// �J�������擾����
	nowHeight = 7.0f;	//	���݂̍���
	horizonDistance = 10.0f;
	lookSpeed = 3.0f;
	lookOffsetY = 1.0f;
	moveSpeed = 5.0f;
	playDistance = 1.0f;
}
void CameraMove::LateUpdate()
{
	UpdatePosition();
	UpdateLook();
}

void CameraMove::ImGuiSetting()
{
	ImGui::Text("UpdatePosition");
	ImGui::DragFloat("nowHeight", &nowHeight);
	ImGui::DragFloat("moveSpeed", &moveSpeed);
	ImGui::DragFloat("playDistance", &playDistance);

	ImGui::Text("UpdateLook");
	ImGui::DragFloat("lookSpeed", &lookSpeed);
	ImGui::DragFloat("lookOffsetY", &lookOffsetY);
	ImGui::DragFloat("lookPlay", &lookPlayDistance);
}