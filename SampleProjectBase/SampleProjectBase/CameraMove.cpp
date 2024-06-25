#include "pch.h"
#include "CameraMove.h"
#include "GameObject.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;

void CameraMove::Roll(Vector2 _input)
{
	// ���ɉ�]����
	centerAngle += -_input.x * rotateSpeed * MainApplication::DeltaTime();
	centerAngle = std::fmod(centerAngle, Mathf::roundDeg);	// �p�x�𐳋K��

	// ������ς���
	nowHeight += -_input.y * heightSpeed * MainApplication::DeltaTime();
	nowHeight = std::clamp(nowHeight, minHeight, maxHeight);	// �͈͓��ɂ����߂�
}

void CameraMove::UpdatePosition()
{
	// �J�������W�̖ڕW���W�����߂�
	Vector3 posTarget = pPlayer->transform.position;
	float rad = DirectX::XMConvertToRadians(centerAngle - 90.0f);
	// XZ���ʂ͒��S�Ƃ̊p�x���狗�������߂�
	posTarget.x += cos(rad) * distance;
	posTarget.z += sin(rad) * distance;
	// �v���C���[����̈��̍�����ۂ�
	posTarget.y += nowHeight;

	// �J�����̍��W���X�V����
	// �ڕW���W�ɐ��`��Ԃ��g�p���ċ߂Â��Ă���
	Transform& t = GetTransform();
	t.position = Vector3::Lerp(t.position, posTarget, moveSpeed * MainApplication::DeltaTime());
}

void CameraMove::UpdateLook()
{
	// �����_�̖ڕW���W
	Vector3 lookTarget = pPlayer->transform.position + Vector3::Up * lookOffsetY;

	// ���`��ԂŌ��݂̒����_��ڕW���W�ɋ߂Â���
	lookPos = Vector3::Lerp(lookPos, lookTarget, lookSpeed * MainApplication::DeltaTime());
	Transform& t = GetTransform();
	t.LookAt(lookPos);
}

void CameraMove::Init()
{
	name = "CameraMove";

	camera = static_cast<Camera*>(gameObject);	// �J�������擾����

	rotateSpeed = 360.0f;	// ��]���x
	heightSpeed = 10.0f;	// �������x
	maxHeight = 3.0f;	// �����̍ő�l�i�v���C���[����j
	nowHeight = 1.0f;	//	���݂̍���
	lookSpeed = 3.0f;
	lookOffsetY = 1.0f;
	moveSpeed = 5.0f;
	distance = 15.0f;

}
void CameraMove::LateUpdate()
{
	UpdatePosition();

	UpdateLook();
}

void CameraMove::SetParameter()
{
	ImGui::Text("Roll");
	ImGui::DragFloat("rotateSpeed{", &rotateSpeed);
	ImGui::DragFloat("centerAngle", &centerAngle);
	ImGui::DragFloat(" heightSpeed", &heightSpeed);
	ImGui::DragFloat("minHeight", &minHeight);
	ImGui::DragFloat("nowHeight", &nowHeight);

	ImGui::Text("UpdatePosition");
	ImGui::DragFloat("moveSpeed", &moveSpeed);
	ImGui::DragFloat("playDistance", &playDistance);
	ImGui::DragFloat("distnce", &distance);

	ImGui::Text("UpdateLook");
	ImGui::DragFloat("lookSpeed", &lookSpeed);
	ImGui::DragFloat("lookOffsetY", &lookOffsetY);

}