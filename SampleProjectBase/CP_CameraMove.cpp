#include "pch.h"
#include "CP_CameraMove.h"
#include "GameObject.h"
#include "CP_Camera.h"

using namespace DirectX::SimpleMath;

void CP_CameraMove::UpdateVector()
{
	Transform transform = GetTransform();

	InputClass& input = MainApplication::GetInput();

	// ���W�ړ�
	Vector2 inputL = input.GetValue("Left");

	// �ړ����������߂�
	moveVec = Vector3::Zero;
	moveVec += transform.Forward() * inputL.y;
	moveVec += transform.Right() * inputL.x;

	// �㏸�E���~
	if (input.GetGamePad().ButtonPress(GamePad::Button::Sankaku) || input.GetKeyboard().GetKey(DIK_SPACE))
		moveVec += transform.Up();
	if (input.GetGamePad().ButtonPress(GamePad::Button::Batsu) || input.GetKeyboard().GetKey(DIK_LCONTROL))
		moveVec -= transform.Up();
	moveVec.Normalize();

	//�@���_�ړ�
	Vector2 inputR = input.GetValue("Right");


	rotateVec.y = inputR.x;
	rotateVec.x = -inputR.y;
	rotateVec.z = -inputR.y;

}

void CP_CameraMove::Move()
{
}


void CP_CameraMove::Init()
{
	name = "CameraMove";

	pCamera = gameObject->GetComponent<CP_Camera>();	// �J�������擾����

	if (pCamera == nullptr)
		HASHI_DEBUG_LOG("Camera�R���|�[�l���g������܂���");

	moveSpeed = 5.0f;
	dashSpeed = 10.0f;
	lookSpeed = 100.0f;
}

void CP_CameraMove::LateUpdate()
{
	UpdateVector();
	Move();
}

void CP_CameraMove::ImGuiSetting()
{
	ImGui::DragFloat("moveSpeed", &moveSpeed);
	ImGui::DragFloat("dashSpeed", &dashSpeed);
	ImGui::DragFloat("lookSpeed", &lookSpeed);
}