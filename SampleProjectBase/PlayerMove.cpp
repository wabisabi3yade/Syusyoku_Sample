#include "pch.h"
#include "PlayerMove.h"
#include "GameObject.h"
#include "Geometory.h"
#include "GameMode.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;

constexpr float MOVE_SPEED(10.0f);

void PlayerMove::Move()
{
	// ���W���X�V
	gameObject->transform.position += moveVec * moveSpeed * MainApplication::DeltaTime();
}

void PlayerMove::Init()
{
	name = "PlayerMove";
	moveSpeed = MOVE_SPEED;

	camera = &GameMode::GetInstance()->GetCamera();
}

void PlayerMove::Update()
{
	Move();	// �ړ�
}

void PlayerMove::LateUpdate()
{
}

void PlayerMove::Draw()
{
	Geometory::SetPosition(gameObject->transform.position + gameObject->transform.Forward() * 5.0f);
	Geometory::DrawCube();

	Geometory::SetPosition(gameObject->transform.position + gameObject->transform.Forward() * 5.0f);
	Geometory::DrawCube();

	Geometory::SetPosition(gameObject->transform.position + gameObject->transform.Forward() * 5.0f);
	Geometory::DrawCube();
}

void PlayerMove::ImGuiSetting()
{
	ImGui::DragFloat("moveSpeed", &moveSpeed);
}

void PlayerMove::DecideMoveVector(DirectX::SimpleMath::Vector2 _input)
{
	// �J�������l�������ړ����������肷��
	moveVec = Vector3::Zero;

	moveVec.x = _input.x;
	moveVec.z = _input.y;
	moveVec.y = 0.0f;
	moveVec.Normalize();
}